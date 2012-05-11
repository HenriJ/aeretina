#ifndef RANGE_H
#define RANGE_H

#include <assert.h>

class RangeIt
{
public:
    RangeIt()
    {
    }

    RangeIt(double val, double delta, unsigned int pos, unsigned int endPos)
        : val_(val), delta_(delta), pos_(pos), endPos_(endPos)
    {
    }

    inline double v() const
    {
        return val_;
    }

    inline unsigned int p() const
    {
        return pos_;
    }

    inline bool check() const
    {
        return (pos_ != endPos_);
    }

    inline void iter()
    {
        val_ += delta_;
        ++pos_;
    }

private:
    double val_;
    double delta_;
    unsigned int pos_;
    unsigned int endPos_;
};

class Range
{
public:
    Range(double begin, double end, double delta)
        : begin_(begin), end_(end), delta_(delta)
    {
        size_ = (unsigned int) (end-begin)/delta + 1;
    }

    inline RangeIt it() const
    {
        return RangeIt(begin_, delta_, 0, size_);
    }

    inline RangeIt it(unsigned int pb, unsigned int pe) const
    {
        assert(pe >= pb && pe < size_);
        return RangeIt(begin_ + pb*delta_, delta_, pb, pe+1);
    }

    inline unsigned int size() const
    {
        return size_;
    }

private:
    double begin_;
    double end_;
    double delta_;

    unsigned int size_;
};

#endif // RANGE_H
