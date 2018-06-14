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
void calculateSize(vector<Point> ROI);
Mat createMask(Size sizeOfMask, vector<Point> ROI);


#endif //TOMATOCLASSIFICATION_FUNCTION_H

/**
 * DONE: Create mask from ROI
 * TODO: If (pixel inside mask) && (color(pixel)=OTHER) => count++
 * TODO: If count>MAX_VALUE => fail,
*/