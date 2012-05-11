#ifndef PUPIL_H
#define PUPIL_H

class Pupil
{
public:
    Pupil()
        : x(64), y(64), r(17) {};
    Pupil(double x, double y, double r)
        : x(x), y(y), r(r) {};

    double x;
    double y;
    double r;

    double confidence;
};

std::ostream& operator<<(std::ostream& output, const Pupil& p)
{
    output << p.x << ", " << p.y << ", " << p.r << ", " << p.confidence;
    return output;
}


#endif // PUPIL_H
