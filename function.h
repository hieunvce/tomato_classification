//
// Created by hieunguyen on 6/14/18.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include "colorProfile.h"

using namespace std;
using namespace cv;

enum STATUS{
    FAIL=-1,
    SKIP_SUCCESS=0,
    RED_NORMAL,
    RED_BAD,
    YELLOW_NORMAL,
    YELLOW_BAD,
    GREEN_NORMAL,
    GREEN_BAD
};

#define MAX_BAD_PIXEL 5000

#ifndef TOMATOCLASSIFICATION_FUNCTION_H
#define TOMATOCLASSIFICATION_FUNCTION_H

Color findColor(Mat LabImage);
Mat segmentImage(Mat LabImage, Color colorID);
vector<Point> detectROI(Mat segImage);
Size2i calculateSize(vector<Point> ROI);
Mat createMask(Size sizeOfMask, vector<Point> ROI);
int countBadPixel(Mat LabImage, Mat maskImage);
STATUS gradeTomato(Color colorID, int nOfBadPixels);
void showInfo(Color tomatoColor, Size2i sizeOfTomato, int nOfBadPixels);
Mat runOnImage(Mat srcImage);

#endif //TOMATOCLASSIFICATION_FUNCTION_H

/**
 * TODO: Use Excel to calculate MAX_BAD_PIXEL
*/