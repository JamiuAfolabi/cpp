#include <iostream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class Node {
public:
    T data;
    Node* next;
    Node* prev;

    Node(T value) : data(value), next(nullptr), prev(nullptr) {}
};

template <typename T>
class Deque {
private:
    Node<T>* front;
    Node<T>* back;
    size_t currentSize;

public:
    Deque() : front(nullptr), back(nullptr), currentSize(0) {}

    // Insert an element at the front
    void insertFront(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->next = front;
        if (front != nullptr) {
            front->prev = newNode;
        }
        front = newNode;
        if (back == nullptr) {
            back = newNode;
        }
        currentSize++;
    }

    // Insert an element at the back
    void insertBack(const T& value) {
        Node<T>* newNode = new Node<T>(value);
        newNode->prev = back;
        if (back != nullptr) {
            back->next = newNode;
        }
        back = newNode;
        if (front == nullptr) {
            front = newNode;
        }
        currentSize++;
    }

    // Access the front element without removing it
    T& peekFront() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        return front->data;
    }

    // Access the back element without removing it
    T& peekBack() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        return back->data;
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
        Node<T>* toRemove = front;
        front = front->next;
        if (front != nullptr) {
            front->prev = nullptr;
        } else {
            back = nullptr;
        }
        currentSize--;
        T value = toRemove->data;
        delete toRemove;
        return value;
    }

    // Remove and return the back element
    T removeBack() {
        if (currentSize == 0) throw std::out_of_range("Deque is empty");
        Node<T>* toRemove = back;
        back = back->prev;
        if (back != nullptr) {
            back->next = nullptr;
        } else {
            front = nullptr;
        }
        currentSize--;
        T value = toRemove->data;
        delete toRemove;
        return value;
    }

    // Iterator class for forward iteration
    class Iterator {
    private:
        Node<T>* current;

    public:
        explicit Iterator(Node<T>* node) : current(node) {}

        Iterator& operator++() {
            current = current->next;
            return *this;
        }

        T& operator*() {
            return current->data;
        }

        bool operator!=(const Iterator& other) const {
            return current != other.current;
        }

        bool operator==(const Iterator& other) const {
            return current == other.current;
        }
    };

    // Begin iterator
    Iterator begin() {
        return Iterator(front);
    }

    // End iterator
    Iterator end() {
        return Iterator(nullptr);
    }

    // ReverseIterator class for reverse iteration
    class ReverseIterator {
    private:
        Node<T>* current;

    public:
        explicit ReverseIterator(Node<T>* node) : current(node) {}

        ReverseIterator& operator++() {
            current = current->prev;
            return *this;
        }

        T& operator*() {
            return current->data;
        }

        bool operator!=(const ReverseIterator& other) const {
            return current != other.current;
        }

        bool operator==(const ReverseIterator& other) const {
            return current == other.current;
        }
    };

    // Begin reverse iterator
    ReverseIterator rbegin() {
        return ReverseIterator(back);
    }

    // End reverse iterator
    ReverseIterator rend() {
        return ReverseIterator(nullptr);
    }

    ~Deque() {
        while (front != nullptr) {
            Node<T>* toRemove = front;
            front = front->next;
            delete toRemove;
        }
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
