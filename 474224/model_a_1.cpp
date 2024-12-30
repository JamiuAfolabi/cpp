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
            paperAvailable = trayCapacity; // Prevent overflow
        }
        std::cout << "Added " << sheets << " sheets of paper. Total available: " << paperAvailable << " sheets.\n";
    }

    void enqueueJob(const PrintJob& job) {
        std::lock_guard<std::mutex> lock(queueMutex);
        printQueue.push(job);
        std::cout << "Added print job: " << job.getDocument() << " (" << job.getPages() << " pages) to the queue.\n";
    }

    void startPrinting() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate wait time before checking the queue
            processJobs();
        }
    }

private:
    void processJobs() {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (!printQueue.empty() && paperAvailable > 0) {
            PrintJob job = printQueue.front();
            printQueue.pop();
            if (job.getPages() <= paperAvailable) {
                std::cout << "Printing job: " << job.getDocument() << " (" << job.getPages() << " pages).\n";
                paperAvailable -= job.getPages();
                std::cout << "Completed printing: " << job.getDocument() << ". Remaining paper: " << paperAvailable << " sheets.\n";
            } else {
                std::cout << "Not enough paper to print job: " << job.getDocument() << ". Moving to next job.\n";
                printQueue.push(job); // Re-add the job to the queue for later processing
            }
        } else if (printQueue.empty()) {
            std::cout << "No print jobs in the queue.\n";
        } else {
            std::cout << "Waiting for paper...\n";
        }
    }

    int trayCapacity;
    int paperAvailable;
    std::queue<PrintJob> printQueue;
    std::mutex queueMutex;
};

int main() {
    Printer printer(10); // Initialize printer with a paper tray capacity of 10 sheets

    // Adding paper to the tray
    printer.addPaper(10);

    // Creating and adding print jobs
    printer.enqueueJob(PrintJob("Document_A", 3));
    printer.enqueueJob(PrintJob("Document_B", 5));
    printer.enqueueJob(PrintJob("Document_C", 7)); // Not enough paper for this job

    // Starting the printing process in a separate thread
    std::thread printThread(&Printer::startPrinting, &printer);

    // Simulating adding more jobs and paper while printing
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printer.addPaper(5); // Add more paper
    printer.enqueueJob(PrintJob("Document_D", 2));

    printThread.join(); // Make sure the printing thread runs until the program is stopped
    return 0;
}