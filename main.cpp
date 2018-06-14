#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    Mat srcImage = imread("./test_image/Yellow/tomato1.jpg",CV_LOAD_IMAGE_COLOR);
    if (!srcImage.data){cout << "ERROR: Can't open image!" << endl; exit(1);}

    Mat LabImage(srcImage.rows,srcImage.cols,CV_8UC3);
    cvtColor(srcImage,LabImage,COLOR_BGR2Lab);
                ///for debug
                namedWindow("lab image",WINDOW_AUTOSIZE);
                imshow("lab image",LabImage);
                cout << "LabImage: rows= " << LabImage.rows << "\t columns= " << LabImage.cols <<endl;
                //end debug
    Mat segImage(srcImage.rows,srcImage.cols,CV_8UC3);

    Color colorID=findColor(LabImage);
    if (colorID != OTHER) {
        segImage = segmentImage(LabImage, colorID);

                ///for debug
                cout << "colorID=" << colorID << endl;
                namedWindow("red segment image", WINDOW_AUTOSIZE);
                imshow("red segment image", segImage);
                cout << "segImage: rows= " << segImage.rows << "\t columns= " << segImage.cols << endl;
                //end debug

        vector<Point> ROI = {Point(0, 0)};
        ROI = detectROI(segImage);
        calculateSize(ROI);

        Scalar color = Scalar(0, 0, 0);
        polylines(srcImage,ROI,true,color,2,8);
        namedWindow("Detected Image", WINDOW_AUTOSIZE);
        imshow("Detected Image", srcImage);

        Size sizeOfMask;
        sizeOfMask.width=srcImage.cols;
        sizeOfMask.height=srcImage.rows;
        Mat maskImage=createMask(sizeOfMask,ROI);
        namedWindow("Mask Image", WINDOW_AUTOSIZE);
        imshow("Mask Image", maskImage);



    } else {
        cout << "Skipped because of 0 tomato detected..." << endl;
    }
    waitKey(0);
    return 0;
}