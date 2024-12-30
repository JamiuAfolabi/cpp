#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(size_t numThreads);
    ~ThreadPool();
    
    // Add a task to the queue
    template<class F>
    void enqueue(F&& f);

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;

    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
    
    void worker();
};

ThreadPool::ThreadPool(size_t numThreads): stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { this->worker(); });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this] { return stop || !tasks.empty(); });
            
            if (stop && tasks.empty())
                return;
            
            task = std::move(tasks.front());
            tasks.pop();
        }
        
        task();
    }
}

template<class F>
void ThreadPool::enqueue(F&& f) {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        tasks.emplace(std::forward<F>(f));
    }
    condition.notify_one();
}

void replaceValueSegment(int* arr, int start, int end, int oldValue, int newValue) {
    for (int* p = arr + start; p < arr + end; ++p) {
        if (*p == oldValue) {
            *p = newValue;
        }
    }
}

void parallelReplaceValue(int* arr, int size, int oldValue, int newValue) {
    const unsigned int numThreads = std::thread::hardware_concurrency();
    ThreadPool pool(numThreads);
    
    int segmentSize = size / numThreads;
    
    for (unsigned int i = 0; i < numThreads; i++) {
        int start = i * segmentSize;
        int end = (i == numThreads - 1) ? size : start + segmentSize; // Handle last segment
        pool.enqueue([arr, start, end, oldValue, newValue] {
            replaceValueSegment(arr, start, end, oldValue, newValue);
        });
    }
}

void printArray(const int *arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    const int SIZE = 20;
    int arr[SIZE] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10,
                     1, 2, 1, 4, 1, 6, 1, 8, 1, 10}; // Sample array
    
    std::cout << "Original Array: ";
    printArray(arr, SIZE);
    
    int oldValue, newValue;

    std::cout << "Enter the value to replace: ";
    std::cin >> oldValue;
    std::cout << "Enter the new value: ";
    std::cin >> newValue;

    parallelReplaceValue(arr, SIZE, oldValue, newValue);

    std::cout << "Modified Array: ";
    printArray(arr, SIZE);

    return 0;
}