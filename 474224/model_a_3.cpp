#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>

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
    Printer(int trayCapacity) : trayCapacity(trayCapacity), paperAvailable(trayCapacity), printing(true) {}

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
        while (printing) {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate wait time before checking the queue
            processJobs();
        }
    }

    void stopPrinting() {
        printing = false;
    }

private:
    static const int ERROR_INSUFFICIENT_PAPER = 1;
    static const int ERROR_QUEUE_FULL = 2;

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
                handleError(ERROR_INSUFFICIENT_PAPER, job);
                printQueue.pop(); // Remove job from the queue
            }
        } else {
            std::cout << "No print jobs in the queue. Waiting...\n";
        }
    }

    void handleError(int errorCode, const PrintJob& job = PrintJob("", 0)) {
        switch (errorCode) {
            case ERROR_INSUFFICIENT_PAPER:
                std::cout << "Error: Insufficient paper to complete the job '" << job.getDocument() << "'.\n";
                waitForPaperAndRetry(job); // Prompt to add paper and retry the job
                break;
            case ERROR_QUEUE_FULL:
                std::cout << "Error: Print queue is full. Please wait or clear jobs.\n";
                // Suggest user options here (optional)
                break;
        }
    }

    void waitForPaperAndRetry(const PrintJob& job) {
        while (true) {
            std::cout << "Would you like to add more paper? (y/n): ";
            char response;
            std::cin >> response;
            if (response == 'y' || response == 'Y') {
                int sheetsToAdd;
                std::cout << "Enter number of sheets to add: ";
                std::cin >> sheetsToAdd;
                addPaper(sheetsToAdd);
                enqueueJob(job); // Retry the job after adding paper
                return;
            } else if (response == 'n' || response == 'N') {
                std::cout << "Skipping print job: " << job.getDocument() << ".\n";
                return; // Skip the job
            } else {
                std::cout << "Invalid response. Please enter 'y' or 'n'.\n";
            }
        }
    }

    int trayCapacity;
    int paperAvailable;
    const int queueCapacity = 5; // Maximum number of print jobs in the queue
    std::queue<PrintJob> printQueue;
    std::mutex queueMutex;
    std::atomic<bool> printing; // Used to control the printing loop
};

int main() {
    Printer printer(10); // Initialize the printer with a paper tray capacity of 10 sheets

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
    printer.enqueueJob(PrintJob("Document_G", 3));

    // Join the printing thread gracefully
    printThread.join();
    return 0;
}