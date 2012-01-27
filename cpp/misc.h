#ifndef MISC_H
#define MISC_H

#include <opencv2/core/core.hpp>

cv::Mat kernel_gaussian(unsigned int half_size, double sigmA);

double cachedExp(unsigned int t, double tau);
double cachedPropExp(unsigned int t, double tau);

#endif // MISC_H
