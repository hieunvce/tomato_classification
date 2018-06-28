#include <iostream>
#include <string.h>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;

vector<Vec3b> setColor={Vec3b(0,0,255),Vec3b(0,255,255),
                        Vec3b(0,255,0),Vec3b(255,255,255)};

int main(int argc, char** argv) {
    /*int runOnDevice=0;
    VideoCapture camera(0);
    Mat srcImage;
    if (argc==2 && !strcmp(argv[1],"--camera")) {
        runOnDevice = 1;
        VideoCapture camera(argv[2]);
    }
    else if (argc==2 && !strcmp(argv[1],"--video")) {
        runOnDevice = 2;
        VideoCapture camera(argv[2]);
    }
    else if (argc==2 && !strcmp(argv[1],"--image")) {
        runOnDevice = 3;
        srcImage=imread(argv[2]);
    } else {
        cout << "Usage:" << endl;
        cout << "\t--camera <cameraID>: Run on camera with <cameraID> (cameraID is integer). If <cameraID>=0: Run on webcam" << endl;
        cout << "\t--video <videoPath>: Run on video at <videoPath>" << endl;
        cout << "\t--image <imagePath>: Run on image at <imagePath>" << endl;
        cout << endl;
        cout << "Default run on Webcame... ESC to exit." << endl;
    }


    if (runOnDevice < 3) {
        int ok = runOnCamera(camera);
        destroyAllWindows();
    } else if (runOnDevice==3){
        Size standardSize(500,500);
        resize(srcImage,srcImage,standardSize);
        Mat d = runOnImage(srcImage);
        namedWindow("d image",CV_WINDOW_AUTOSIZE);
        imshow("d image",d);
        waitKey(0);
    }*/
    Mat srcImage=imread("images/1.jpg");
    Size standardSize(500,500);
    resize(srcImage,srcImage,standardSize);
    imshow("source image",srcImage);
    Mat d = runOnImage(srcImage);
    namedWindow("d image",CV_WINDOW_AUTOSIZE);
    imshow("d image",d);
    waitKey(0);
    return 0;
}