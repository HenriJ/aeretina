#ifndef MAT_H
#define MAT_H

template <class T>
class Mat
{
public:
    Mat(int width) : width_(width), size_(width*width) {
        m = new T[size_];
    }
    ~Mat() {
        delete m;
    }

    inline int width() const { return width_; }
    inline int size() const { return size_; }

    inline T& operator()(const int x, const int y) {
        return m[y * width_ + x];
    }

    inline T& operator[](const int i) {
        return m[i];
    }

private:
    int width_;
    int size_;

    double * m;
};

#endif // MAT_H
