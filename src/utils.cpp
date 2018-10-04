//
// Created by hieunguyen on 9/29/18.
//

#include "utils.h"
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

void help(){
    std::cout << "Help:" << std::endl;
    std::cout << "Run on camera: ./TomatoClassification [-c | --camera]" << std::endl;
    std::cout << "Run on image: ./TomatoClassification [-i | --image] [path to image]" << std::endl;
    std::cout << "Run on video: ./TomatoClassification [-v | --video] [path to video]" << std::endl;
}

int distanceBetween2Points(const Point &A, const Point &B) {
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}

