#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "function.h"

using namespace std;
using namespace cv;


int main(int argc, char **argv) {
    Mat colorImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    long prevTickCount = getTickCount();

    Size2i tomatoSize(0,0);
    float gValue=0.f;
    calculateSizeAndGValue(colorImage,tomatoSize,gValue);

    double executionTime = (getTickCount() * 1.00 - prevTickCount * 1.00) / (getTickFrequency() * 1.00);
    cout << argv[1] << "\t" << tomatoSize.width << "\t" << tomatoSize.height << "\t" << gValue
                                                                                     << "\t" << executionTime << endl;
    imwrite(argv[1],colorImage);
    //waitKey(0);
    return 0;
}
