#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

template <typename T>
class Deque {
private:
    T* data;           // Pointer to the underlying array
    size_t capacity;   // Total capacity of the array
    size_t frontIndex; // Index of the front element
    size_t backIndex;  // Index of the last element
    size_t currentSize; // Current number of elements

    // Resize the underlying array
    void resize(size_t newCapacity) {
        T* newData = new T[newCapacity];
        size_t j = 0;

        // Copy elements to the new array
        for (size_t i = frontIndex; i != backIndex; i = (i + 1) % capacity) {
            newData[j++] = data[i];
        }
        newData[j] = data[backIndex]; // Copy the last element

        frontIndex = 0;
        backIndex = j;
        capacity = newCapacity;

        // Cleanup
        delete[] data;
        data = newData;
    }

public:
    // Nested iterator class
    class Iterator {
    private:
        Deque<T>* deque; // Pointer to the deque
        size_t index;     // Current index

    public:
        Iterator(Deque<T>* d, size_t idx) : deque(d), index(idx) {}

        // Dereference operator
        T& operator*() {
            return deque->data[index];
        }

        // Prefix increment
        Iterator& operator++() {
            index = (index + 1) % deque->capacity;
            if (index == deque->backIndex)
                index = deque->frontIndex; // Move to back of current size
            return *this;
        }

        // Equality operator
        bool operator==(const Iterator& other) const {
            return deque == other.deque && index == other.index;
        }

        // Inequality operator
        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructor
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
        frontIndex = (frontIndex == 0 ? capacity : frontIndex) - 1; // Move forward circularly
        data[frontIndex] = value;
        currentSize++;
        if (currentSize == 1) { // When deque had no previous elements
            backIndex = frontIndex;
        }
    }

    // Insert an element at the back
    void insertBack(const T& value) {
        if (currentSize == capacity) {
            resize(capacity * 2); // Double the capacity
        }
        data[backIndex] = value;
        backIndex = (backIndex + 1) % capacity; // Move back circularly
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
        if (currentSize == 0) { // Reset pointers if empty
            frontIndex = 0;
            backIndex = 0;
        }
        return value;
    }

    // Remove and return the back element
    T removeBack() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        backIndex = (backIndex == 0 ? capacity : backIndex) - 1;
        T value = data[backIndex];
        currentSize--;
        if (currentSize == 0) { // Reset pointers if empty
            frontIndex = 0;
            backIndex = 0;
        }
        return value;
    }

    // Returns an iterator to the beginning
    Iterator begin() {
        return Iterator(this, frontIndex);
    }

    // Returns an iterator to the end
    Iterator end() {
        return Iterator(this, backIndex);
    }

    // Const iterator methods for const Deque
    class ConstIterator {
    private:
        const Deque<T>* deque; // Pointer to the deque
        size_t index;          // Current index

    public:
        ConstIterator(const Deque<T>* d, size_t idx) : deque(d), index(idx) {}

        const T& operator*() const {
            return deque->data[index];
        }

        ConstIterator& operator++() {
            index = (index + 1) % deque->capacity;
            if (index == deque->backIndex)
                index = deque->frontIndex; // Move to back of current size
            return *this;
        }

        bool operator==(const ConstIterator& other) const {
            return deque == other.deque && index == other.index;
        }

        bool operator!=(const ConstIterator& other) const {
            return !(*this == other);
        }
    };

    ConstIterator cbegin() const {
        return ConstIterator(this, frontIndex);
    }
    ConstIterator cend() const {
        return ConstIterator(this, backIndex);
    }
};

int main() {
    Deque<int> deque;
    
    // Insert some elements
    deque.insertBack(1);
    deque.insertBack(2);
    deque.insertBack(3);
    deque.insertFront(0);

    // Using std::for_each to print all elements
    std::for_each(deque.begin(), deque.end(), [](const int& value) {
        std::cout << value << " ";
    });
    std::cout << std::endl;

    return 0;
}