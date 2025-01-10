#include <iterator>
#include <algorithm>
#include <iostream>

template <typename T>
class Deque {
    // ... (Same code as before)
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
    // Iterator class
    class iterator : public std::iterator<std::bidirectional_iterator_tag, T> {
    private:
        Deque<T>* deque;
        size_t index;

    public:
        iterator(Deque<T>* deque, size_t index) : deque(deque), index(index) {}

        iterator& operator++() {
            index = (index + 1) % deque->capacity;
            return *this;
        }

        iterator operator++(int) {
            iterator old(*this);
            operator++();
            return old;
        }

        iterator& operator--() {
            index = (index == 0 ? deque->capacity : index) - 1;
            return *this;
        }

        iterator operator--(int) {
            iterator old(*this);
            operator--();
            return old;
        }

        bool operator==(const iterator& other) const {
            return deque == other.deque && index == other.index;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

        T& operator*() {
            return deque->data[index];
        }
    };

    // Begin and end iterators for forward and reverse iteration
    iterator begin() {
        return iterator(this, frontIndex);
    }

    iterator end() {
        return iterator(this, backIndex == frontIndex ? capacity : backIndex);
    }

    iterator rbegin() {
        return iterator(this, (backIndex == 0 ? capacity : backIndex) - 1);
    }

    iterator rend() {
        return iterator(this, (frontIndex == 0 ? capacity : frontIndex) - 1);
    }

    // ... (Same code as before)
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
};
int main() {
    Deque<int> deque;
    deque.insertBack(1);
    deque.insertBack(2);
    deque.insertBack(3);
    deque.insertFront(0);

    // Printing using range-based for loop:
    std::cout << "Forward Iteration: \n";
    for (auto it = deque.begin(); it != deque.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Reverse Iteration: \n";
    for (auto it = deque.rbegin(); it != deque.rend(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Printing using std::for_each:
    std::cout << "Forward Iteration using std::for_each: \n";
    std::for_each(deque.begin(), deque.end(), [](int value) {
        std::cout << value << " ";
    });
    std::cout << "\n";
