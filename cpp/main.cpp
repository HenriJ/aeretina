#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "doublemat.h"

using namespace std;

const unsigned char WIDTH = 128;

DoubleMat gaussian_gen(unsigned int size, double sigmA) {
	DoubleMat mat(2*size + 1);

	unsigned int cells = mat.side * mat.side;

	int shift = (int) size;
	int x = -shift;
	int y = -shift;

	sigmA = -sigmA;

	for (unsigned int i = 0; i < cells; ++i) {
		mat.dat[i] = exp(double( x*x + y*y ) / sigmA);

		++x;
		if (x > shift) {
			x = -shift;
			++y;
		}
	}

	mat.normalize();
	return mat;
}

DoubleMat gaussiandiff_gen(unsigned int size, double sigmA, double sigmB, double ratio) {
	DoubleMat mat(2*size + 1);

	unsigned int cells = mat.side * mat.side;

	int shift = (int) size;
	int x = -shift;
	int y = -shift;

	sigmA = -(sigmA*sigmA);
	sigmB = -(sigmB*sigmB);

	for (unsigned int i = 0; i < cells; ++i) {
		mat.dat[i] = exp(double( x*x + y*y ) / sigmA) - ratio * exp(double( x*x + y*y ) / sigmB);

		++x;
		if (x > shift) {
			x = -shift;
			++y;
		}
	}

	mat.normalize();
	return mat;
}

struct camEvent {
	unsigned char x;
	unsigned char y;
	char p;
	unsigned int t;

	camEvent(unsigned char x, unsigned char y,
		     char p, unsigned int t) 
		: x(x), y(y), p(p), t(t) {}
};

struct bpEvent {
	double v;
	unsigned int t;

	bpEvent(double v, unsigned int t)
		: v(v), t(t) {}
};

struct bpParams {
	unsigned int timeWindow;
	double* spatialTransform;
};


unsigned char rand_char(unsigned char max) {
	return (unsigned char) (((double) rand() / RAND_MAX) * max);
}

// def diff_gauss_kern(size = 2, sigmA = 0.9, sigmB = 1., ratio = 1.):
// 	""" Returns a normalized 2D kernel of a gaussian difference """
// 	x, y = sp.mgrid[-size:size+1, -size:size+1]
// 	sigmA = double(sigmA**2)
// 	sigmB = double(sigmB**2)
// 	g = sp.exp( -(x**2 + y**2) / sigmA ) - ratio * sp.exp( -(x**2 + y**2) / sigmB )
// 	return g / abs(g.sum())

void bp_feed(vector<bpEvent> bp, camEvent e, bpParams* params) {
// 	STwidth = ST.shape[0]
// 	shift = int(np.floor(STwidth / 2))
// 	for x in range(STwidth):
// 		for y in range(STwidth):
// 			bipolarX = e.x - shift + x
// 			bipolarY = e.y - shift + y
// 			// Check the boundaries
// 			if (bipolarX >= 0 and bipolarX < width and bipolarY >= 0 and bipolarY < width):
// 				cell = BP[e.x - shift + x][e.y - shift + y]
// 				// TODO: convert the list to a heap to avoid extensive search of outdated datas
// 				for d in cell:
// 					if (e.t - d[1] >= maxT):
// 						cell.remove(d)
// 				cell.append( (ST[x, y]*e.p, e.t) )
}



int main(int argc, char *argv[])
{
	// Generation of random events
	//srand ( time(NULL) );
	srand(0); // Repetability
	vector<camEvent> aer;
	for (unsigned int i=0; i < 1000000; ++i) {
		unsigned char x = rand_char(WIDTH);
		unsigned char y = rand_char(WIDTH);
		char p = rand_char(2) * 2 - 1;

		aer.push_back(camEvent(x, y, p, i));
	}


	// Generation of ONe parameters
	DoubleMat gD = gaussiandiff_gen(2, 1, 0.8, 1);
	cout << gD << endl;

	// Simulation
	vector<bpEvent> bpONe[WIDTH][WIDTH];
	for (unsigned int i=0; i < aer.size(); ++i) {
		
	}

    return 0;
}