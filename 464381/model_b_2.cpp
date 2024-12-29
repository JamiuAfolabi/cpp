class Point {
private:
    int x_;
    int y_;

public:
    inline int getX() const {
        return x_;
    }

    inline void setX(int x) {
        x_ = x;
    }

    inline int getY() const {
        return y_;
    }

    inline void setY(int y) {
        y_ = y;
    }
};