#ifndef PUPIL_H
#define PUPIL_H

class Pupil
{
public:
    Pupil()
        : x(64), y(64), a(17), b(17), theta(0) {};
    Pupil(double x, double y, double a, double b, double theta)
        : x(x), y(y), a(a), b(b), theta(theta) {};

    double x;
    double y;
    double a;
    double b;
    double theta;

    double confidence;
};

std::ostream& operator<<(std::ostream& output, const Pupil& p)
{
    output << p.x << ", " << p.y << ", " << p.a << ", " << p.b << ", " << p.theta << ", " << p.confidence;
    return output;
}


#endif // PUPIL_H
