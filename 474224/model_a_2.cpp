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
        if (printQueue.size() >= queueCapacity) {
            handleError(ERROR_QUEUE_FULL);
            return;
        }
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
                printQueue.pop(); // Remove job from the queue and handle the error
            }
        } else {
            std::cout << "No print jobs in the queue. Waiting...\n";
        }
    }

    void handleError(int errorCode) {
        switch (errorCode) {
            case ERROR_INSUFFICIENT_PAPER:
                std::cout << "Error: Insufficient paper to complete the job.\n";
                // Potential recovery: ask to add more paper
                break;
            case ERROR_QUEUE_FULL:
                std::cout << "Error: Print queue is full. Please wait or clear jobs.\n";
                // Potential recovery: remove the oldest job or wait
                break;
            case ERROR_INVALID_JOB:
                std::cout << "Error: Invalid print job encountered.\n";
                // Potential recovery: skip the job
                break;
        }
    }

    int trayCapacity;
    int paperAvailable;
    const int queueCapacity = 5; // Maximum number of print jobs in the queue
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
    printer.enqueueJob(PrintJob("Document_C", 8)); // Not enough paper for this job
    printer.enqueueJob(PrintJob("Document_D", 1));
    printer.enqueueJob(PrintJob("Document_E", 2));
    printer.enqueueJob(PrintJob("Document_F", 4)); // Queue full error

    // Starting the printing process in a separate thread
    std::thread printThread(&Printer::startPrinting, &printer);

    // Simulating adding more jobs and paper while printing
    std::this_thread::sleep_for(std::chrono::seconds(3));
    printer.addPaper(5); // Add more paper
    // Attempting to add more jobs
    printer.enqueueJob(PrintJob("Document_G", 3));
    
    printThread.join(); // Join the printing thread
    return 0;
}