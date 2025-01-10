#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Deque {
private:
    T* data;            // Pointer to the underlying array
    size_t capacity;    // Total capacity of the array
    size_t frontIndex;  // Index of the front element
    size_t backIndex;   // Index of the last element
    size_t currentSize; // Current number of elements

    // Resize the underlying array
    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];

        // Copy elements to the new array
        for (size_t i = 0; i < currentSize; ++i) {
            newData[i] = data[(frontIndex + i) % capacity];
        }

        frontIndex = 0;
        backIndex = currentSize;
        capacity = newCapacity;

        // Cleanup
        delete[] data;
        data = newData;
    }

public:
    // Iterator class for forward iteration
    class Iterator {
    private:
        T* data;
        size_t index;

    public:
        Iterator(T* data, size_t index) : data(data), index(index) {}

        Iterator& operator++() {
            index++;
            return *this;
        }

        T& operator*() {
            return data[index];
        }

        bool operator!=(const Iterator& other) const {
            return index != other.index;
        }

        bool operator==(const Iterator& other) const {
            return index == other.index;
        }
    };

    // ReverseIterator class for reverse iteration
    class ReverseIterator {
    private:
        T* data;
        size_t index;

    public:
        ReverseIterator(T* data, size_t index) : data(data), index(index) {}

        ReverseIterator& operator++() {
            index--;
            return *this;
        }

        T& operator*() {
            return data[index];
        }

        bool operator!=(const ReverseIterator& other) const {
            return index != other.index;
        }

        bool operator==(const ReverseIterator& other) const {
            return index == other.index;
        }
    };

    Deque(size_t initialCapacity = 4)
        : capacity(initialCapacity), frontIndex(0), backIndex(0), currentSize(0) {
        data = new T[capacity];
    }

    ~Deque() {
        delete[] data;
    }

    // Insert an element at the front
    void insertFront(const T& value) {
        if (currentSize == capacity) {
            resize(capacity * 2); // Double the capacity
        }

        // Shift all elements to the right to make space at the front
        for (size_t i = currentSize; i > 0; --i) {
            data[(frontIndex + i) % capacity] = data[(frontIndex + i - 1) % capacity];
        }

        data[frontIndex] = value;
        backIndex = (backIndex + 1) % capacity;
        currentSize++;
    }

    // Insert an element at the back
    void insertBack(const T& value) {
        if (currentSize == capacity) {
            resize(capacity * 2); // Double the capacity
        }

        data[backIndex] = value;
        backIndex = (backIndex + 1) % capacity;
        currentSize++;
    }

    // Access the front element without removing it
    T& peekFront() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        return data[frontIndex];
    }

    // Access the back element without removing it
    T& peekBack() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        return data[(backIndex == 0 ? capacity : backIndex) - 1];
    }

    // Get the current size of the deque
    size_t size() const {
        return currentSize;
    }

    // Check if the deque is empty
    bool isEmpty() const {
        return currentSize == 0;
    }

    // Remove and return the front element
    T removeFront() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        T value = data[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        currentSize--;
        return value;
    }

    // Remove and return the back element
    T removeBack() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        backIndex = (backIndex == 0 ? capacity : backIndex) - 1;
        T value = data[backIndex];
        currentSize--;
        return value;
    }

    // Begin iterator
    Iterator begin() {
        return Iterator(data, frontIndex);
    }

    // End iterator
    Iterator end() {
        return Iterator(data, frontIndex + currentSize);
    }

    // Begin reverse iterator
    ReverseIterator rbegin() {
        return ReverseIterator(data, (backIndex == 0 ? capacity : backIndex) - 1);
    }

    // End reverse iterator
    ReverseIterator rend() {
        return ReverseIterator(data, frontIndex - 1);
    }
};

// Example usage
int main() {
    Deque<int> deque;

    deque.insertBack(1);
    deque.insertBack(2);
    deque.insertFront(0);
    deque.insertBack(3);

    // Print using std::for_each for forward iteration
    std::cout << "Deque elements: ";
    std::for_each(deque.begin(), deque.end(), [](int& val) {
        std::cout << val << " ";
    });
    std::cout << std::endl; // Should print 0 1 2 3

    // Print using std::for_each for reverse iteration
    std::cout << "Deque elements in reverse: ";
    std::for_each(deque.rbegin(), deque.rend(), [](int& val) {
        std::cout << val << " ";
    });
    std::cout << std::endl; // Should print 3 2 1 0

    return 0;
}