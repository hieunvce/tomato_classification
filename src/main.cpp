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
    //long prevTickCount = getTickCount();
    high_resolution_clock::time_point t1 = high_resolution_clock::now();

    Size2i tomatoSize(0,0);
    float gValue=0.f;
    calculateSizeAndGValue(colorImage,tomatoSize,gValue);
    cout << "Tomato Size = " << tomatoSize << endl;
    cout << "gValue = " << gValue << endl;

    //double executionTime = (getTickCount() * 1.00 - prevTickCount * 1.00) / (getTickFrequency() * 1.00);
    //cout << "execution Time = " << executionTime << " s" << endl;
    high_resolution_clock::time_point t2 = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(t2-t1).count();
    cout << "duration = " << (float)duration/1000000.f << endl;
    return 0;
}
