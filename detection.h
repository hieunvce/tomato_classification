//
// Created by henry on 2/26/18.
//

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#ifndef TOMATOCLASSIFICATION_DETECTION_H
#define TOMATOCLASSIFICATION_DETECTION_H

RotatedRect detection(Mat srcImage, Mat segImage);

#endif //TOMATOCLASSIFICATION_DETECTION_H
