#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include "function.h"
#include "svm.h"

using namespace std;
using namespace cv;

#define MIN_TOMATO_AREA     100         //Minimum area of region to be a tomato
#define THRESHOLD_VALUE     2830000     //HEX Value for background
#define MIN_DEPTH           3           //If depth > MIN_DEPTH => will be removed
#define MIN_DISTANCE        10000       //If 2 defects too far, they will be ignored
#define NUMBER_OF_ATTRIBUTE 3

struct svm_model *model;
struct svm_node *x;

int distanceBetween2Points(const Point &A, const Point &B) {
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}

vector<Point> removeFromContour(const vector<Point> &contour, const vector<int> &defectsIndex, int &minDistance) {
    minDistance = INT_MAX;
    int startIndex, endIndex;

    // Find nearest defects
    for (int i = 0; i < defectsIndex.size(); ++i) {
        for (int j = i + 1; j < defectsIndex.size(); ++j) {
            int distance = distanceBetween2Points(contour[defectsIndex[i]], contour[defectsIndex[j]]);
            if (distance < minDistance) {
                minDistance = distance;
                startIndex = defectsIndex[i];
                endIndex = defectsIndex[j];
            }
        }
    }

    if (minDistance > MIN_DISTANCE)
        return contour;
    // Check if intervals are swapped
    if (startIndex <= endIndex) {
        int len1 = endIndex - startIndex;
        int len2 = static_cast<int>(contour.size() - endIndex + startIndex);
        if (len2 < len1) {
            std::swap(startIndex, endIndex);
        }
    } else {
        int len1 = startIndex - endIndex;
        int len2 = static_cast<int>(contour.size() - startIndex + endIndex);
        if (len1 < len2) {
            std::swap(startIndex, endIndex);
        }
    }

    // Remove unwanted points
    vector<Point> contourAfterRemoving;
    if (startIndex <= endIndex) {
        contourAfterRemoving.insert(contourAfterRemoving.end(), contour.begin(), contour.begin() + startIndex);
        contourAfterRemoving.insert(contourAfterRemoving.end(), contour.begin() + endIndex, contour.end());
    } else {
        contourAfterRemoving.insert(contourAfterRemoving.end(), contour.begin() + endIndex,
                                    contour.begin() + startIndex);
    }
    return contourAfterRemoving;
}

void calculateSize(cv::Mat& image, cv::Size2i& tomatoSize, cv::Mat& ROI){
    Mat thresholdImage(image.size(), CV_8UC1);

    for (int y = 0; y < image.rows; ++y) {
        const uchar *ROIData = image.ptr<uchar>(y);
        auto *thresholdImageData = thresholdImage.ptr<uchar>(y);
        for (int x = 0; x < image.cols; ++x) {
            int b = *ROIData++;
            int g = *ROIData++;
            int r = *ROIData++;

            int hexValue = (r * 65536) + (g * 256) + b;
            if (hexValue < THRESHOLD_VALUE) {
                *thresholdImageData++ = 0;
            } else {
                *thresholdImageData++ = 255;
            }
        }
    }
    vector<vector<Point> > contours;
    findContours(thresholdImage, contours, RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
    vector<Point> tomatoBoundary;

    if (!contours.empty()) {
        int maxContourArea = static_cast<int>(contourArea(contours[0]));
        int largestContourIndex = 0;
        for (int i = 1; i < contours.size(); ++i) {
            int ctArea = static_cast<int>(contourArea(contours[i]));
            if (ctArea > maxContourArea) {
                maxContourArea = ctArea;
                largestContourIndex = i;
            }
        }

        if (maxContourArea >= MIN_TOMATO_AREA) {
            tomatoBoundary = contours[largestContourIndex];

            //------------------------------------------------------------------
            vector<int> convexHullIndex;
            convexHull(tomatoBoundary, convexHullIndex, false, false);
            vector<Vec4i> convexityDefectIndex;
            convexityDefects(tomatoBoundary, convexHullIndex, convexityDefectIndex);
            int minDistance = INT_MAX;

            while (true) {
                vector<int> defectsIndex;
                for (const Vec4i &v:convexityDefectIndex) {
                    float depth = static_cast<float>(v[3]) / 256.f;

                    if (depth > MIN_DEPTH) {
                        defectsIndex.push_back(v[2]);
                    }
                }

                if (defectsIndex.size() < 2) {
                    break;
                }
                // If I have more than 2 defects, remove the points between the two nearest defects
                tomatoBoundary = removeFromContour(tomatoBoundary, defectsIndex, minDistance);

                if (minDistance >= MIN_DISTANCE) {
                    break;
                }
                convexHull(tomatoBoundary, convexHullIndex, false, false);
                convexityDefects(tomatoBoundary, convexHullIndex, convexityDefectIndex);
            }

            // bounding Box
            cv::RotatedRect boundingBox;
            boundingBox = minAreaRect(tomatoBoundary);
            tomatoSize.width = (int) boundingBox.size.width;
            tomatoSize.height = (int) boundingBox.size.height;

            // Draw bounding box, for debug only
            Point2f rectPoints[4];
            boundingBox.points(rectPoints);
            for (int i = 0; i < 4; i++) {
                line(image, rectPoints[i], rectPoints[(i + 1) % 4], Scalar(255, 255, 255), 2, 8);
            }
        }
    }

    if (tomatoSize.area() != 0) {
        Mat mask = Mat::zeros(image.size(), CV_8UC1);

        vector<vector<Point> > cntArray;
        // Push tomatoBoundary to a vector to pass as argument to drawContours function
        cntArray.push_back(tomatoBoundary);
        drawContours(mask, cntArray, -1, (255), CV_FILLED);
        //imshow("mask",mask);

        for (int y = 0; y < mask.rows; ++y) {
            uchar *roi_data = ROI.ptr<uchar>(y);
            const uchar *mask_data = mask.ptr<uchar>(y);
            for (int x = 0; x < ROI.cols; ++x) {
                //check if pixel outside mask
                if (*mask_data != 255) {
                    *roi_data=0;
                    roi_data++;
                    *roi_data=0;
                    roi_data++;
                    *roi_data=0;
                    roi_data++;
                } else {
                    roi_data+=3;
                }
                mask_data++;
            }
        }
        //imshow("ROI",ROI);
        //imwrite("a.jpg",ROI);
    }
}
std::vector<int> find3PeaksHistogram(cv::Mat& ROI){
    int redArray[255]={0};
    int greenArray[255]={0};
    int blueArray[255]={0};
    
    for (int y=0;y<ROI.rows;++y){
        const uchar *pixel_data = ROI.ptr<uchar>(y);
        for (int x=0;x<ROI.cols;++x) {
            int b = *pixel_data++;
            int g = *pixel_data++;
            int r = *pixel_data++;
            if (r != 0 && g != 0 && b != 0) {
                redArray[r] += 1;
                greenArray[g] += 1;
                blueArray[b] += 1;
            }
        }
    }

    vector<int> peak(3,0);

    peak[0]=std::distance(redArray,std::max_element(redArray,redArray+255));
    peak[1]=std::distance(greenArray,std::max_element(greenArray,greenArray+255));
    peak[2]=std::distance(blueArray,std::max_element(blueArray,blueArray+255));

    return peak;
}
double predictColor(vector<int> feature){
    //feature[0]=red peak, feature[1]=green peak, feature[2]=blue peak
    double predict_label;
    if ((model=svm_load_model("tomato_classifier.model"))==0){
        cout << "Can't open model file!" << endl;
        exit(1);
    }
    x=(struct svm_node *)malloc((NUMBER_OF_ATTRIBUTE+1)*sizeof(struct svm_node));
    x[0].index=1; x[0].value=feature[0];
    x[1].index=2; x[1].value=feature[1];
    x[2].index=3; x[2].value=feature[2];
    x[3].index=-1; x[3].value=0;
    //predict
    predict_label = svm_predict(model,x);
    svm_free_and_destroy_model(&model);
    free(x);
    return predict_label;
}
