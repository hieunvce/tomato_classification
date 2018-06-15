#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    Mat srcImage = imread("./test_image/Bad/asd.png",CV_LOAD_IMAGE_COLOR);
    if (!srcImage.data){cout << "ERROR: Can't open image!" << endl; exit(1);}

    Mat LabImage(srcImage.rows,srcImage.cols,CV_8UC3);
    cvtColor(srcImage,LabImage,COLOR_BGR2Lab);

    Mat segImage(srcImage.rows,srcImage.cols,CV_8UC3);

    Color colorID=findColor(LabImage);

    if (colorID != OTHER) {
        segImage = segmentImage(LabImage, colorID);

        vector<Point> ROI = {Point(0, 0)};
        ROI = detectROI(segImage);
        Size2i tomatoSize;
        tomatoSize=calculateSize(ROI);
        cout << endl << "Tomato size: Height: " << tomatoSize.height << " | Width: " << tomatoSize.width << endl;

        Size sizeOfMask;
        sizeOfMask.width=srcImage.cols;
        sizeOfMask.height=srcImage.rows;
        Mat maskImage=createMask(sizeOfMask,ROI);

        int badPixels=0;
        badPixels= countBadPixel(LabImage, maskImage);
        cout << "Number of Bad pixels: " << badPixels << endl;

        //-------For debugging-----------------------------------------
        Scalar color = Scalar(0, 0, 0);
        polylines(srcImage,ROI,true,color,2,8);
        namedWindow("Detected Image", WINDOW_AUTOSIZE);
        imshow("Detected Image", srcImage);
        //------End---------------------------------------------------

    } else {
        cout << "Skipped because of 0 tomato detected..." << endl;
    }
    waitKey(0);
    return 0;
}