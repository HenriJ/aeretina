#include "misc.h"

#include <cstdlib>
#include <vector>
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
