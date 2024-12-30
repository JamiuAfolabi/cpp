#include <iostream>
#include <queue>
#include <string>
#include <thread>
#include <chrono>
#include <mutex>

enum class PrinterError {
    NO_ERROR,
    OUT_OF_PAPER,
    NO_JOBS_IN_QUEUE,
    INTERNAL_ERROR
};

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
        if (sheets <= 0) {
            std::cout << "Error: Invalid amount of paper to add.\n";
            return;
        }
        paperAvailable += sheets;
        if (paperAvailable > trayCapacity) {
            paperAvailable = trayCapacity;
        }
        std::cout << "Added " << sheets << " sheets of paper. Total available: " << paperAvailable << " sheets.\n";
    }

    void enqueueJob(const PrintJob& job) {
        if (job.getPages() <= 0) {
            std::cout << "Error: Invalid number of pages in the print job.\n";
            return;
        }
        std::lock_guard<std::mutex> lock(queueMutex);
        printQueue.push(job);
        std::cout << "Added print job: " << job.getDocument() << " (" << job.getPages() << " pages) to the queue.\n";
    }

    void startPrinting() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            PrinterError error = processJobs();
            handleError(error);
        }
    }

private:
    PrinterError processJobs() {
        std::lock_guard<std::mutex> lock(queueMutex);

        if (printQueue.empty()) {
            return PrinterError::NO_JOBS_IN_QUEUE;
        }

        PrintJob job = printQueue.front();
        printQueue.pop();

        if (job.getPages() > paperAvailable) {
            return PrinterError::OUT_OF_PAPER;
        }

        std::cout << "Printing job: " << job.getDocument() << " (" << job.getPages() << " pages).\n";
        paperAvailable -= job.getPages();
        std::cout << "Completed printing: " << job.getDocument() << ". Remaining paper: " << paperAvailable << " sheets.\n";

        return PrinterError::NO_ERROR;
    }

    void handleError(PrinterError error) {
        switch (error) {
            case PrinterError::OUT_OF_PAPER:
                std::cout << "Error: Out of paper. Please add paper.\n";
                break;
            case PrinterError::NO_JOBS_IN_QUEUE:
                std::cout << "Error: No print jobs in the queue.\n";
                break;
            case PrinterError::INTERNAL_ERROR:
                std::cout << "Error: Internal printer error.\n";
                break;
            default:
                break;
        }
    }

    int trayCapacity;
    int paperAvailable;
    std::queue<PrintJob> printQueue;
    std::mutex queueMutex;
};

int main() {
    Printer printer(10);

    printer.addPaper(10);

    printer.enqueueJob(PrintJob("Document_A", 3));
    printer.enqueueJob(PrintJob("Document_B", 0)); // Invalid job
    printer.enqueueJob(PrintJob("Document_C", 7));

    std::thread printThread(&Printer::startPrinting, &printer);