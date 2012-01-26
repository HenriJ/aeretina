#ifndef MISC_H
#define MISC_H

#include <opencv2/core/core.hpp>

unsigned char rand_char(unsigned char max);

cv::Mat kernel_gaussian(unsigned int half_size, double sigmA);

#endif // MISC_H
