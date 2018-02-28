//
// Created by henry on 2/28/18.
//
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

#ifndef TOMATOCLASSIFICATION_CLASSIFYBYCOLOR_H
#define TOMATOCLASSIFICATION_CLASSIFYBYCOLOR_H

/* Create a mask image, which have an filled ellipse.
 * For each pixel in the original image, whether
 * the mask pixel at the same image coordinates is set (then the pixel is inside the object).
 */
Mat createMaskImage(Mat srcImage, RotatedRect boundingBox);
void calculateEachColorPercentage(Mat srcImage, Mat maskImage);

#endif //TOMATOCLASSIFICATION_CLASSIFYBYCOLOR_H
