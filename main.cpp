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

    //Calculate ellipse from rotated rectangle and draw to srcImage
    Size axes;
    axes.height = boundingBox.size.height/2;
    axes.width = boundingBox.size.width/2;

    Scalar color = Scalar(0,0,0);
    cout << "Tomato size: Height: " << axes.height << " | Width: " << axes.width << endl;
    ellipse(srcImage,boundingBox.center,axes,boundingBox.angle,0.0,360.0,color,2,8,0);

    Mat mask = createMaskImage(srcImage,boundingBox);
    calculateEachColorPercentage(srcImage, mask);

    namedWindow("Detected Image",WINDOW_AUTOSIZE);
    imshow("Detected Image",srcImage);

    waitKey(0);
    return 0;
}