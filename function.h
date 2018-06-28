//
// Created by hieunguyen on 6/14/18.
//
#include <opencv2/opencv.hpp>
#include <iostream>
#include "colorProfile.h"

using namespace std;
using namespace cv;

enum STATUS{ FAIL=-1, SKIP_SUCCESS=0,
    RED_NORMAL, RED_BAD, YELLOW_NORMAL, YELLOW_BAD, GREEN_NORMAL, GREEN_BAD
};

#define MAX_BAD_PIXEL 5000

#ifndef TOMATOCLASSIFICATION_FUNCTION_H
#define TOMATOCLASSIFICATION_FUNCTION_H

void SegmentImage(Mat LabImage_8UC3, Mat &segImage_8U, Color &colorID);
vector<Point> detectROI(Mat segImage);
Size2i calculateSize(vector<Point> ROI);
Mat createMask(Size sizeOfMask, vector<Point> ROI);
int countBadPixel(Mat LabImage, Mat maskImage);
STATUS gradeTomato(Color colorID, int nOfBadPixels);
void showInfo(Size2i sizeOfTomato, STATUS grade);
Mat runOnImage(Mat srcImage);
int runOnCamera(VideoCapture camera);

#endif //TOMATOCLASSIFICATION_FUNCTION_H