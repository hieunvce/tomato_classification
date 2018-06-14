//
// Created by henry on 3/4/18.
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#ifndef TOMATOCLASSIFICATION_COLORPROFILE_H
#define TOMATOCLASSIFICATION_COLORPROFILE_H

/**
 * Define an enum for COLOR for easily return in function
 */
enum Color{ RED, YELLOW, GREEN, OTHER };

bool isRed(int a, int b);
bool isGreen(int a, int b);
bool isYellow( int a, int b);
Color color(int a, int b);

#endif //TOMATOCLASSIFICATION_COLORPROFILE_H
