#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

// Simple thread pool implementation
class ThreadPool {
public:
    ThreadPool(int numThreads) : threads(numThreads) {
        for (int i = 0; i < numThreads; i++) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(mtx);
                        cv.wait(lock, [this] { return tasks.empty() || shutdown; });
                        if (shutdown && tasks.empty()) {
                            return;
                        }
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
            });
        }
    }

    void enqueueTask(std::function<void()> task) {
        std::unique_lock<std::mutex> lock(mtx);
        tasks.emplace_back(std::move(task));
        cv.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mtx);
            shutdown = true;
        }
        cv.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mtx;
    std::condition_variable cv;
    bool shutdown = false;
};

void replaceValueSegment(int* arr, int start, int end, int oldValue, int newValue) {
    for (int* p = arr + start; p < arr + end; ++p) {
        if (*p == oldValue) {
            *p = newValue;
        }
    }
}

void parallelReplaceValue(int* arr, int size, int oldValue, int newValue, ThreadPool& threadPool) {
    const int numThreads = threadPool.threads.size();
    int segmentSize = size / numThreads;

    for (int i = 0; i < numThreads; i++) {
        int start = i * segmentSize;
        int end = (i == numThreads - 1) ? size : start + segmentSize;
        threadPool.enqueueTask([arr, start, end, oldValue, newValue] {
            replaceValueSegment(arr, start, end, oldValue, newValue);
        });
    }
}

void printArray(const int* arr, int size) {
    for (int i = 0; i < size; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int SIZE = 20000000; // Increased size to test performance improvements
    int arr[SIZE] = {1, 2, 3, 4, 5, /* Populate array with actual data */}; 

    std::cout << "Original Array: ";
    printArray(arr, SIZE); 

    int oldValue, newValue; 
    std::cout << "Enter the value to replace: ";
    std::cin >> oldValue; 
    std::cout << "Enter the new value: ";
    std::cin >> newValue; 
    
    // Set up a thread pool with the number of threads you want to use
    ThreadPool threadPool(std::thread::hardware_concurrency());