#include <iostream>
#include <opencv2/opencv.hpp>
#include "segmentation.h"
#include "detection.h"
#include "classifyByColor.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {

    Mat srcImage = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    Mat redSegImage(srcImage.rows,srcImage.cols,CV_8UC3);
    Mat yellowSegImage(srcImage.rows,srcImage.cols,CV_8UC3);
    Mat greenSegImage(srcImage.rows,srcImage.cols,CV_8UC3);

    redSegImage = segmentation(srcImage,1);
    namedWindow("RedSeg",WINDOW_AUTOSIZE);
    imshow("RedSeg",redSegImage);
    yellowSegImage = segmentation(srcImage,2);
    greenSegImage = segmentation(srcImage,3);
    float redArea,yellowArea,greenArea;
    redArea=yellowArea=greenArea=0;
    RotatedRect redBoundingBox = detection(srcImage,redSegImage,redArea);
    RotatedRect yellowBoundingBox = detection(srcImage,yellowSegImage,yellowArea);
    RotatedRect greenBoundingBox = detection(srcImage,greenSegImage,greenArea);
    RotatedRect boundingBox;
    if (redArea>=yellowArea && redArea>=greenArea)
        boundingBox = redBoundingBox;
    else if (yellowArea>=redArea && yellowArea>=greenArea)
        boundingBox = yellowBoundingBox;
    else
        boundingBox = greenBoundingBox;

    Mat mask = createMaskImage(srcImage,boundingBox);
    calculateEachColorPercentage(srcImage, mask);

    waitKey(0);
    return 0;
}