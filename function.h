//
// Created by hieunguyen on 6/14/18.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include "colorProfile.h"

using namespace std;
using namespace cv;

#ifndef TOMATOCLASSIFICATION_FUNCTION_H
#define TOMATOCLASSIFICATION_FUNCTION_H

Color findColor(Mat LabImage);
Mat segmentImage(Mat LabImage, Color colorID);
RotatedRect detectROI(Mat segImage);
#endif //TOMATOCLASSIFICATION_FUNCTION_H
