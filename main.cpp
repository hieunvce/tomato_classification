#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {

    //Mat srcImage = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    Mat srcImage = imread("./test_image/Bad/l3.jpg",CV_LOAD_IMAGE_COLOR);
    Mat LabImage(srcImage.rows,srcImage.cols,CV_8UC3);
    cvtColor(srcImage,LabImage,COLOR_BGR2Lab);
                ///for debug
                namedWindow("lab image",WINDOW_AUTOSIZE);
                imshow("lab image",LabImage);
                cout << "LabImage: rows= " << LabImage.rows << "\t columns= " << LabImage.cols <<endl;
                //end debug
    Mat segImage(srcImage.rows,srcImage.cols,CV_8UC3);

    Color colorID=findColor(LabImage);
    segImage = segmentImage(srcImage,colorID);

                ///for debug
                namedWindow("red segment image",WINDOW_AUTOSIZE);
                imshow("red segment image",segImage);
                cout << "segImage: rows= " << segImage.rows << "\t columns= " << segImage.cols <<endl;
                //end debug

    RotatedRect boundingBox;
    boundingBox=detectROI(segImage);
    //Calculate ellipse from rotated rectangle and draw to srcImage
    Size axes;
    axes.height = boundingBox.size.height/2;
    axes.width = boundingBox.size.width/2;

    Scalar color = Scalar(0,0,0);
    cout << endl << "Tomato size: Height: " << axes.height << " | Width: " << axes.width << endl;
    ellipse(srcImage,boundingBox.center,axes,boundingBox.angle,0.0,360.0,color,2,8,0);

    namedWindow("Detected Image",WINDOW_AUTOSIZE);
    imshow("Detected Image",srcImage);

    waitKey(0);
    return 0;
}