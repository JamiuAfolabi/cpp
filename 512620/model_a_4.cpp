#include <iostream>
#include <stdexcept>

template <typename T>
class Deque {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& value) : data(value), next(nullptr), prev(nullptr) {}
    };

    Node* front;       // Pointer to the front node
    Node* back;        // Pointer to the back node
    size_t currentSize; // Current number of elements

public:
    Deque() : front(nullptr), back(nullptr), currentSize(0) {}

    ~Deque() {
        while (!isEmpty()) {
            removeFront();
        }
    }

    // Insert an element at the front
    void insertFront(const T& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = back = newNode;
        } else {
            newNode->next = front;
            front->prev = newNode;
            front = newNode;
        }
        currentSize++;
    }

    // Insert an element at the back
    void insertBack(const T& value) {
        Node* newNode = new Node(value);
        if (isEmpty()) {
            front = back = newNode;
        } else {
            newNode->prev = back;
            back->next = newNode;
            back = newNode;
        }
        currentSize++;
    }

    // Access the front element without removing it
    T& peekFront() {
        if (isEmpty()) throw std::out_of_range("Deque is empty");
        return front->data;
    }

    // Access the back element without removing it
    T& peekBack() {
        if (isEmpty()) throw std::out_of_range("Deque is empty");
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
        if (isEmpty()) throw std::out_of_range("Deque is empty");
        Node* temp = front;
        T value = front->data;
        front = front->next;
        if (front) {
            front->prev = nullptr;
        } else {
            back = nullptr; // Deque is empty now
        }
        delete temp;
        currentSize--;
        return value;
    }

    // Remove and return the back element
    T removeBack() {
        if (isEmpty()) throw std::out_of_range("Deque is empty");
        Node* temp = back;
        T value = back->data;
        back = back->prev;
        if (back) {
            back->next = nullptr;
        } else {
            front = nullptr; // Deque is empty now
        }
        delete temp;
        currentSize--;
        return value;
    }

    // Iterator class for forward iteration
    class Iterator {
    private:
        Node* current;

    public:
        Iterator(Node* node) : current(node) {}

        Iterator& operator++() {
            if (current) {
                current = current->next;
            }
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

    // ReverseIterator class for reverse iteration
    class ReverseIterator {
    private:
        Node* current;

    public:
        ReverseIterator(Node* node) : current(node) {}

        ReverseIterator& operator++() {
            if (current) {
                current = current->prev;
            }
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

    // Begin iterator
    Iterator begin() {
        return Iterator(front);
    }

    // End iterator
    Iterator end() {
        return Iterator(nullptr);
    }

    // Begin reverse iterator
    ReverseIterator rbegin() {
        return ReverseIterator(back);
    }

    // End reverse iterator
    ReverseIterator rend() {
        return ReverseIterator(nullptr);
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