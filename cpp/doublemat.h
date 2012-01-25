#ifndef DOUBLEMAT_H
#define DOUBLEMAT_H

#include <iostream>

class DoubleMat {
public:
	double * dat;
	unsigned int side;

	DoubleMat(unsigned int side);
	DoubleMat(double * dat, unsigned int side);

	~DoubleMat();

	double abssum() const;
	void normalize();
};

std::ostream& operator<<(std::ostream& output, const DoubleMat& mat);

#endif // DOUBLEMAT_H