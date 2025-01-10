#include <iostream>
using namespace std;

class Deque {
private:
    int* arr;
    int front;
    int rear;
    int size; // Actual size of the underlying array
    int capacity;
    int utilization_threshold; // Threshold for dynamic resizing

public:
    Deque(int threshold = 25) // Default utilization threshold = 25%
    {
        arr = new int[1];
        size = 0;
        capacity = 1;
        front = 0;
        rear = 0;
        utilization_threshold = capacity * threshold / 100;
    }

    // Finalizer to free the dynamically allocated memory
    ~Deque()
    {
        delete[] arr;
    }

    // Enqueue at the front
    void push_front(int element)
    {
        if (size + 1 > capacity) {
            resize(capacity * 2);
        }

        front = (front - 1 + capacity) % capacity;
        arr[front] = element;
        size++;
    }

    // Enqueue at the rear
    void push_back(int element)
    {
        if (size + 1 > capacity) {
            resize(capacity * 2);
        }

        arr[rear] = element;
        rear = (rear + 1) % capacity;
        size++;
    }

    // Dequeue from the front
    int pop_front()
    {
        if (is_empty()) {
            cerr << "Deque is empty." << endl;
            exit(1);
        }

        int ret = arr[front];
        front = (front + 1) % capacity;
        size--;

        // Check for capacity reduction
        if (size < capacity * (50 - utilization_threshold) / 100) {
            resize(capacity / 2);
        }

        return ret;
    }

    // Dequeue from the rear
    int pop_back()
    {
        if (is_empty()) {
            cerr << "Deque is empty." << endl;
            exit(1);
        }

        int ret = arr[(rear - 1 + capacity) % capacity];
        rear = (rear - 1 + capacity) % capacity;
        size--;

        // Check for capacity reduction
        if (size < capacity * (50 - utilization_threshold) / 100) {
            resize(capacity / 2);
        }

        return ret;
    }

    // Get the front element without dequeueing
    int front_element() const
    {
        if (is_empty()) {
            cerr << "Deque is empty." << endl;
            exit(1);
        }
        return arr[front];
    }

    // Get the rear element without dequeueing
    int back_element() const
    {
        if (is_empty()) {
            cerr << "Deque is empty." << endl;
            exit(1);
        }
        return arr[(rear - 1 + capacity) % capacity];
    }

    bool is_empty() const
    {
        return size == 0;
    }

    int size() const
    {
        return size;
    }

private:
    // Function to dynamically resize the array
    void resize(int new_capacity)
    {
        int* temp = new int[new_capacity];

        int index = front;
        for (int i = 0; i < size; i++) {
            temp[i] = arr[index];
            index = (index + 1) % capacity;
        }

        delete[] arr;
        arr = temp;
        front = 0;
        rear = size - 1;
        capacity = new_capacity;
    }
};
 