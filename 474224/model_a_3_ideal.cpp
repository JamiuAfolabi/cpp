#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

class PrintJob {
public:
    PrintJob(std::string document, int pages)
        : document(document), pages(pages) {}

    std::string getDocument() const { return document; }
    int getPages() const { return pages; }

private:
    std::string document;
    int pages;
};

class Printer {
public:
    Printer(int trayCapacity) : trayCapacity(trayCapacity), paperAvailable(trayCapacity) {}

    void addPaper(int sheets) {
        paperAvailable += sheets;
        if (paperAvailable > trayCapacity) {
            paperAvailable = trayCapacity;
        }
        std::cout << "Added " << sheets << " sheets of paper. Total available: " << paperAvailable << " sheets.\n";
    }

    void enqueueJob(const PrintJob& job) {
        std::lock_guard<std::mutex> lock(queueMutex);
        if (printQueue.size() >= queueCapacity) {
            handleError(ERROR_QUEUE_FULL);
            return;
        }
        printQueue.push(job);
        std::cout << "Added print job: " << job.getDocument() << " (" << job.getPages() << " pages) to the queue.\n";
    }

    void startPrinting() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            processJobs();
        }
    }

private:
    static const int ERROR_INSUFFICIENT_PAPER = 1;
    static const int ERROR_QUEUE_FULL = 2;
    static const int ERROR_INVALID_JOB = 3;

    void processJobs() {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (!printQueue.empty()) {
            PrintJob job = printQueue.front();
            if (job.getPages() <= paperAvailable) {
                printQueue.pop();
                std::cout << "Printing job: " << job.getDocument() << " (" << job.getPages() << " pages).\n";
                paperAvailable -= job.getPages();
                std::cout << "Completed printing: " << job.getDocument() << ". Remaining paper: " << paperAvailable << " sheets.\n";
            } else {
                handleError(ERROR_INSUFFICIENT_PAPER);
                printQueue.pop();
            }
        } else {
            std::cout << "No print jobs in the queue. Waiting...\n";
        }
    }

    void handleError(int errorCode) {
        switch (errorCode) {
            case ERROR_INSUFFICIENT_PAPER:
                std::cout << "Error: Insufficient paper to complete the job.\n";
                char choice;
                std::cout << "Do you want to add more paper? (y/n): ";
                std::cin >> choice;
                if (choice == 'y' || choice == 'Y') {
                    int sheets;
                    std::cout << "Enter the number of sheets to add: ";
                    std::cin >> sheets;
                    addPaper(sheets);
                } else {
                    std::cout << "Skipping the current print job.\n";
                }
                break;
            case ERROR_QUEUE_FULL:
                std::cout << "Error: Print queue is full. Please wait or clear jobs.\n";
                break;
            case ERROR_INVALID_JOB:
                std::cout << "Error: Invalid print job encountered.\n";
                break;
        }
    }

    int trayCapacity;
    int paperAvailable;
    const int queueCapacity = 5;
    std::queue<PrintJob> printQueue;
    std::mutex queueMutex;
};

int main() {
    Printer printer(10);

    printer.addPaper(10);

    printer.enqueueJob(PrintJob("Document_A", 3));
    printer.enqueueJob(PrintJob("Document_B", 5));
    printer.enqueueJob(PrintJob("Document_C", 8));
    printer.enqueueJob(PrintJob("Document_D", 1));
    printer.enqueueJob(PrintJob("Document_E", 2));
    printer.enqueueJob(PrintJob("Document_F", 4));

    std::thread printThread(&Printer::startPrinting, &printer);

    std::this_thread::sleep_for(std::chrono::seconds(3));
    printer.addPaper(5);
    printer.enqueueJob(PrintJob("Document_G", 3));

    printThread.join();
    return 0;
}
