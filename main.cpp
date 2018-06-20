#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;

vector<Vec3b> setColor={Vec3b(0,0,255),Vec3b(0,255,255),
                        Vec3b(0,255,0),Vec3b(255,255,255)};

int main(int argc, char** argv) {
    Mat srcImage = imread("./test_image/samples/1.jpg",CV_LOAD_IMAGE_COLOR);//BGR
    if (!srcImage.data){cout << "ERROR: Can't open image!" << endl; exit(1);}
    Size standardSize(500,500);
    resize(srcImage,srcImage,standardSize);

    //-------------debugging-------------------------------------------------------
    Mat LabImage(srcImage.rows, srcImage.cols, CV_8UC3);
    cvtColor(srcImage, LabImage, COLOR_BGR2Lab);//0<L<100, −127<a<127, −127<b<127
    namedWindow("lab image",WINDOW_AUTOSIZE);
    imshow("lab image",LabImage);
    //----------------------------------------------------------------------------

    Mat segImage(srcImage.rows, srcImage.cols, CV_8UC3);
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            int l=*lab_data++;
            l=l*100/255;
            int a=*lab_data++-128;
            int b=*lab_data++-128;
            if (isRed(l,a,b))
                segImage.at<Vec3b>(i,j)=setColor[RED];
            else if (isGreen(l,a,b))
                segImage.at<Vec3b>(i,j)=setColor[GREEN];
            else if (isYellow(l,a,b))
                segImage.at<Vec3b>(i,j)=setColor[YELLOW];
            else
                segImage.at<Vec3b>(i,j)=setColor[OTHER];
        }
    }
    //----------------------------------------------------------------------------
    namedWindow("red segment image", WINDOW_AUTOSIZE);
    imshow("red segment image", segImage);
    //-----------debugging--------------------------------------------------------

    /*STATUS status=runOnImage(srcImage);
    cout << "TOMATO GRADE: ";
    switch (status){
        case RED_NORMAL:
            cout << "RED_NORMAL"<< endl;
            break;
        case RED_BAD:
            cout << "RED_BAD"<< endl;
            break;
        case YELLOW_NORMAL:
            cout << "YELLOW_NORMAL"<< endl;
            break;
        case YELLOW_BAD:
            cout << "YELLOW_BAD"<< endl;
            break;
        case GREEN_NORMAL:
            cout << "GREEN_NORMAL" << endl;
            break;
        case GREEN_BAD:
            cout << "GREEN_BAD" << endl;
            break;
        case SKIP_SUCCESS:
            cout << "SKIP_SUCCESS" << endl;
            break;
        case FAIL:
            cout << "FAIL" << endl;
            exit(1);
    }
     */
    waitKey(0);
    return 0;
}