
#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <chrono>

using namespace std;

class PrintJob {
public:
    string filename;
    int pages;

    PrintJob(string filename, int pages) : filename(filename), pages(pages) {}
};

class Printer {
private:
    queue<PrintJob> printQueue;
    mutex mtx;
    condition_variable cv;
    bool running = true;
    int paperTrayPages = 100;

public:
    void enqueueJob(PrintJob job) {
        unique_lock<mutex> lock(mtx);
        printQueue.push(job);
        cv.notify_one();
        cout << "Job " << job.filename << " enqueued. " << "Queue size: " << printQueue.size() << endl;
    }

    void dequeueJob() {
        unique_lock<mutex> lock(mtx);
        cv.wait(lock, [this] { return !printQueue.empty() || !running; });

        if (!running) {
            return;
        }

        PrintJob currentJob = printQueue.front();
        printQueue.pop();
        cout << "Printing job: " << currentJob.filename << " (" << currentJob.pages << " pages)" << endl;

        // Simulate printing time
        for (int i = 0; i < currentJob.pages; i++) {
            if (paperTrayPages <= 0) {
                cout << "Paper tray empty. Printing paused." << endl;
                cv.wait(lock, [this] { return paperTrayPages > 0; });
                cout << "Paper added. Printing resumed." << endl;
            }
            paperTrayPages--;
            cout << "Printing page " << i + 1 << " of " << currentJob.pages << endl;
            this_thread::sleep_for(chrono::milliseconds(500)); // Simulate 0.5 seconds printing time
        }
    }

    void addPaper(int pages) {
        unique_lock<mutex> lock(mtx);
        paperTrayPages += pages;
        cv.notify_one();
        cout << "Paper added. Paper tray pages: " << paperTrayPages << endl;
    }

    void stop() {
        unique_lock<mutex> lock(mtx);
        running = false;
        cv.notify_one();
    }
};

int main() {
    Printer printer;

    thread printingThread([&printer] {
        while (true) {
            printer.dequeueJob();
            if (printer.printQueue.empty()) {
                cout << "Printer idle." << endl;
            }
        }
    });

    cout << "Printer simulation started." << endl;

    printer.enqueueJob(PrintJob("document1.pdf", 10));
    printer.enqueueJob(PrintJob("image.jpg", 1));
    printer.enqueueJob(PrintJob("presentation.ppt", 20));

    this_thread::sleep_for(chrono::seconds(2));  // Simulate some time passing

    printer.addPaper(50);

    printer.enqueueJob(PrintJob("report.docx", 8));

    this_thread::sleep_for(chrono::seconds(5));  // Simulate more time passing

    printer.stop();
    printingThread.join();

    cout << "Printer simulation terminated." << endl;
    return 0;
}
