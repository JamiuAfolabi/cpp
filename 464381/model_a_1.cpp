#include <iostream>

class Rectangle {
private:
    double width;
    double height;

public:
    // Inline getter for width
    inline double getWidth() const {
        return width;
    }

    // Inline setter for width
    inline void setWidth(double w) {
        if (w >= 0) {
            width = w;
        }
    }

    // Inline getter for height
    inline double getHeight() const {
        return height;
    }

    // Inline setter for height
    inline void setHeight(double h) {
        if (h >= 0) {
            height = h;
        }
    }

    // Function to calculate area
    double area() const {
        return getWidth() * getHeight();
    }
};

int main() {
    Rectangle rect;

    rect.setWidth(5.0);
    rect.setHeight(3.0);

    std::cout << "Width: " << rect.getWidth() << std::endl;
    std::cout << "Height: " << rect.getHeight() << std::endl;
    std::cout << "Area: " << rect.area() << std::endl;

    return 0;
}