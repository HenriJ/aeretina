#ifndef DOUBLEMAT_H
#define DOUBLEMAT_H

#include <iostream>
using namespace std;

class DoubleMat
{
public:
    DoubleMat(int width);
    DoubleMat(const char * path) { init(path); }
    DoubleMat(std::string path) { init(path.c_str()); }

    DoubleMat(const DoubleMat & dm);
    DoubleMat & operator= (const DoubleMat & dm);

    virtual ~DoubleMat();

    inline int width() const { return width_; }

    inline int size() const { return size_; }

    inline double & v(int x, int y) const { return mat[y * width_ + x]; }

    inline double c(int x, int y) const
    {
        x += half_width_;
        y += half_width_;

        int pos = y * width_ + x;

        if (pos >= size_ || pos < 0) {
            return 0;
        }
        return mat[y * width_ + x];
    }

private:
    void init(const char * path);

    int width_;
    int half_width_;
    int size_;

    int * ref;
    double * mat;
};

#endif // DOUBLEMAT_H
