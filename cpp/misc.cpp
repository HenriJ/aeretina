#include <cstdlib>
#include <vector>
#include "misc.h"
#include <iostream>

using namespace cv;
using namespace std;

Mat kernel_gaussian(unsigned int half_size, double sigmA) {
    int size = 2 * half_size + 1;
    Mat K(size, size, DataType<double>::type);

    int shift = (int) half_size;

    sigmA = -sigmA;

    int x, y;

    double sum = 0;

    for(int i = 0; i < size; i++) {
        x = i - shift;
        for(int j = 0; j < size; j++) {
            y = j - shift;
            double temp = exp((x*x + y*y) / sigmA);
            K.at<double>(i, j) = temp;
            sum += temp;
        }
    }

    K = K / abs(sum);

    return K;
}

double cachedExp(unsigned int t, double tau) {
    static vector<double> cache(100000);

    if (cache[t] == 0) {
        cache[t] = exp(-t / tau);
        if (t >= 100000) {
            std::cout << "cachedExp overflow" << "\n";
        }
    }

    return cache[t];
}

double cachedPropExp(unsigned int t, double tau) {
    static vector<double> cache(100000);

    if (cache[t] == 0) {
        double temp = t / tau;
        cache[t] = temp * exp(-temp);
        if (t >= 100000) {
            std::cout << "cachedPropExp overflow" << "\n";
        }
    }

    return cache[t];
}
