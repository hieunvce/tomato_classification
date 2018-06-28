#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;

vector<Vec3b> setColor={Vec3b(0,0,255),Vec3b(0,255,255),
                        Vec3b(0,255,0),Vec3b(255,255,255)};

int main(int argc, char** argv) {
    VideoCapture camera("clips/Demo.mp4");//pass 0 to open webcame
    int ok = runOnCamera(camera);
    destroyAllWindows();

    /*Mat srcImage=imread("images/samples/1.jpg",CV_LOAD_IMAGE_COLOR);
    Size standardSize(500,500);
    resize(srcImage,srcImage,standardSize);
    Mat d = runOnImage(srcImage);
    namedWindow("d image",CV_WINDOW_AUTOSIZE);
    imshow("d image",d);
    waitKey(0);
     */

    return 0;
}