#include <cstdio>
#include <stdlib.h>
#include "doublemat.h"

DoubleMat::DoubleMat(unsigned int side)
	: side(side)
{
	dat = new double[side * side];
}

DoubleMat::DoubleMat(double * dat, unsigned int side)
	: dat(dat), side(side)
{
}

DoubleMat::~DoubleMat()
{
	delete[] dat;
}

double DoubleMat::abssum() const
{
	double sum = 0;
	for (unsigned int i = 0; i < side * side; ++i) {
		sum += dat[i];
	}
	return abs(sum);
}

void DoubleMat::normalize()
{
	double as = abssum();
	for (unsigned int i = 0; i < side * side; ++i) {
		dat[i] /= as;
	}
}

std::ostream& operator<<(std::ostream& output, const DoubleMat& mat)
{
	unsigned int side = mat.side;
	unsigned int cells = side * side;
    output << "Size : " <<  side << "*" << side << "; ";
    output << "AbsSum : " << mat.abssum() << "; ";
	for (unsigned int i = 0; i < cells; ++i) {
		if (i % side == 0) {
			output << std::endl;
		}
		printf("%8.5f ", mat.dat[i]);
	}
    return output;
}
