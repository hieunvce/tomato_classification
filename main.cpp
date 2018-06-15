#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;


int main(int argc, char** argv) {
    Mat srcImage = imread(argv[1],CV_LOAD_IMAGE_COLOR);
    if (!srcImage.data){cout << "ERROR: Can't open image!" << endl; exit(1);}
    Size standardSize(500,500);
    resize(srcImage,srcImage,standardSize);

    STATUS status=runOnImage(srcImage);
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
    waitKey(0);
    return 0;
}