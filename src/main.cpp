#include <iostream>
#include <algorithm>
#include <chrono>
#include <opencv2/opencv.hpp>
#include "function.h"
#include "utils.h"

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
            Size2i tomatoSize;
            float gValue;
            while (1) {
                high_resolution_clock::time_point t1 = high_resolution_clock::now();
                cap >> colorImage;
                if (colorImage.empty()) {
                    break;
                }
                calculateSizeAndGValue(colorImage, tomatoSize, gValue);
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
            Size2i tomatoSize;
            float gValue;
            calculateSizeAndGValue(colorImage, tomatoSize, gValue);
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(t2 - t1).count();
            cout << duration / 1000000.f << endl;

            cout << colorImage.size() << endl;
            imshow("image",colorImage);
            waitKey(0);
        } else {
            help();
        }
    }
    destroyAllWindows();
    return 0;
}
