#ifndef RANGE_H
#define RANGE_H

class Range
{
public:
    Range(double begin, double end, double delta)
        : begin_(begin), end_(end), delta(delta)
    {
        size_ = (unsigned int) (end-begin)/delta + 1;
        val = begin;
        pos_ = 0;
    }

    inline double begin()
    {
        val = begin_;
        pos_ = 0;
        return val;
    }

    inline double iter()
    {
        val += delta;
        ++pos_;
        return val;
    }

    inline unsigned int pos()
    {
        return pos_;
    }

    inline double end() const
    {
        return end_;
    }

    inline unsigned int size()
    {
        return size_;
    }

private:
    double begin_;
    double end_;
    double delta;

    double val;
    unsigned int pos_;

    unsigned int size_;
};

#endif // RANGE_H
