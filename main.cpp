#include <iostream>
#include <opencv2/opencv.hpp>
#include "segmentation.h"
#include "detection.h"
#include "classifyByColor.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    // Read image
    Mat srcImage = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    Mat segImage(srcImage.rows,srcImage.cols,CV_8UC3);
    Mat detectedImage=srcImage.clone();

    segmentation(srcImage,segImage);
    RotatedRect boundingBox = detection(srcImage,segImage);
    Mat mask = createMaskImage(srcImage,boundingBox);
    calculateEachColorPercentage(srcImage, mask);

    waitKey(0);
    return 0;
}