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
vector<Point> detectROI(Mat segImage);
Size2i calculateSize(vector<Point> ROI);
Mat createMask(Size sizeOfMask, vector<Point> ROI);
int countBadPixel(Mat LabImage, Mat maskImage);

int runOnImage(Mat image);

#endif //TOMATOCLASSIFICATION_FUNCTION_H

/**
 * TODO: If count>MAX_VALUE => fail,
*/