#include <iostream>
#include <algorithm>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "function.h"
#include "utils.h"
#include "svm.h"

using namespace std;
using namespace std::chrono;
using namespace cv;


int main(int argc, char **argv) {
    if (argc < 2) {
        help();
    } else {
        if (std::string(argv[1]) == "-c" || std::string(argv[1]) == "--camera"
            || std::string(argv[1]) == "-v" || std::string(argv[1]) == "--video") {
            VideoCapture cap;
            if (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--video") {
                cap.open(argv[2]);
            } else {
                cap.open(0);
            }
            if (!cap.isOpened()) {
                cout << "Error opening camera." << endl;
                return -1;
            }
            Mat colorImage;
            Mat ROI;
            Size2i tomatoSize;
            float gValue;
            RotatedRect boundingBox;
            while (1) {
                high_resolution_clock::time_point t1 = high_resolution_clock::now();
                cap >> colorImage;
                ROI=colorImage.clone();
                if (colorImage.empty()) {
                    break;
                }
                calculateSize(colorImage, tomatoSize, ROI);
                high_resolution_clock::time_point t2 = high_resolution_clock::now();
                auto duration = duration_cast<microseconds>(t2 - t1).count();
                cout << duration / 1000000.f << endl;

                cout << colorImage.size() << endl;
                if (waitKey(1) == 27)
                    break;
            }
            cap.release();
        } else if (std::string(argv[1]) == "-i" || std::string(argv[1]) == "--image") {
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            Mat colorImage = imread(argv[2]);
            Mat ROI=colorImage.clone();

            Size2i tomatoSize;
            calculateSize(colorImage, tomatoSize, ROI);
            vector<int> feature(3,0);
            feature = find3PeaksHistogram(ROI);
            cout << "3 1:" << feature[0] << " 2:" << feature[1] << " 3:" << feature[2] << endl;
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(t2 - t1).count();
            double predictValue = predictColor(feature);
            cout << "Predict value: " << predictValue << endl;
            cout << "Execute time:" << duration / 1000000.f << endl;
            imshow("image",colorImage);
            waitKey(0);
        } else {
            help();
        }
    }
    destroyAllWindows();
    return 0;
}
