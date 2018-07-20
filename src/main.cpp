#include <iostream>
#include <algorithm>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "function.h"

using namespace std;
using namespace std::chrono;
using namespace cv;


int main(int argc, char **argv) {
    Mat colorImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    Size2i tomatoSize(0,0);
    float gValue=0.f;
    calculateSizeAndGValue(colorImage,tomatoSize,gValue);

    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2-t1).count();
    cout << argv[1] << "\t" << tomatoSize.width << "\t" << tomatoSize.height << "\t" << gValue << "\t" << duration << endl;
    //imwrite(argv[1],colorImage);
    return 0;
}
