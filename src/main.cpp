#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

#define MIN_TOMATO_AREA     100         //Minimum area of region to be a tomato
#define THRESHOLD_VALUE     2830000     //HEX Value for background
#define MIN_DEPTH           3           //If depth > MIN_DEPTH => will be removed
#define MIN_DISTANCE        10000       //If 2 defects too far, they will be ignored

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

int main(int argc, char **argv) {
    Mat colorImage = imread(argv[1], CV_LOAD_IMAGE_COLOR);
    long prevTickCount = getTickCount();
    Mat thresholdImage(colorImage.size(), CV_8UC1);
    Mat gChannelImage(colorImage.size(), CV_8UC1);
    for (int y = 0; y < colorImage.rows; ++y) {
        const uchar *colorImageData = colorImage.ptr<uchar>(y);
        auto *thresholdImageData = thresholdImage.ptr<uchar>(y);
        auto *gChannelImageData = gChannelImage.ptr<uchar>(y);
        for (int x = 0; x < colorImage.cols; ++x) {
            int b = *colorImageData++;
            int g = *colorImageData++;
            int r = *colorImageData++;

            *gChannelImageData++ = static_cast<uchar>(g);

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
    Size2i tomatoSize(0, 0);
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
            int minDistance=INT_MAX;

            while (true) {
                vector<int> defectsIndex;
                for (const Vec4i &v:convexityDefectIndex) {
                    float depth = static_cast<float>(v[3]) / 256.f;

                    if (depth > MIN_DEPTH) {
                        defectsIndex.push_back(v[2]);

//                        //------for debugging----------------------------------------
//                        cout << "depth = " << depth << endl;
//                        int startidx = v[0];
//                        Point ptStart(tomatoBoundary[startidx]);
//                        int endidx = v[1];
//                        Point ptEnd(tomatoBoundary[endidx]);
//                        int faridx = v[2];
//                        Point ptFar(tomatoBoundary[faridx]);
//
//                        line(colorImage, ptStart, ptEnd, Scalar(255, 0, 0), 1);
//                        line(colorImage, ptStart, ptFar, Scalar(0, 255, 0), 1);
//                        line(colorImage, ptEnd, ptFar, Scalar(0, 0, 255), 1);
//                        circle(colorImage, ptFar, 4, Scalar(127, 127, 255), 2);
//                        //-----end debugging-----------------------------------------

                    }
                }

                if (defectsIndex.size() < 2){
                    break;
                }
                // If I have more than 2 defects, remove the points between the two nearest defects
                tomatoBoundary = removeFromContour(tomatoBoundary, defectsIndex,minDistance);

                if (minDistance >= MIN_DISTANCE) {
                    break;
                }
                convexHull(tomatoBoundary, convexHullIndex, false, false);
                convexityDefects(tomatoBoundary, convexHullIndex, convexityDefectIndex);
            }


            //------------------------------------------------------------------
            RotatedRect boundingBox;
            boundingBox = minAreaRect(tomatoBoundary);
            Point2f rectPoints[4];
            boundingBox.points(rectPoints);
            for (int i = 0; i < 4; i++)
                line(colorImage, rectPoints[i], rectPoints[(i + 1) % 4], Scalar(255, 255, 255), 2, 8);
            tomatoSize.width = (int) boundingBox.size.width;
            tomatoSize.height = (int) boundingBox.size.height;
        }
    }

    cout << "Tomato size: " << tomatoSize << endl;

    if (tomatoSize.area() != 0) {
        Mat mask = Mat::zeros(colorImage.size(), CV_8UC1);
        vector<vector<Point> > cntArray;
        // Push tomatoBoundary to a vector to pass as argument to drawContours function
        cntArray.push_back(tomatoBoundary);
        drawContours(mask, cntArray, -1, (255), CV_FILLED);
        //namedWindow("Mask Image", WINDOW_NORMAL);
        //imshow("Mask Image", mask);

        int sumOfGreenValues = 0;
        int numberOfMaskPixel = 0;
        for (int y = 0; y < gChannelImage.rows; ++y) {
            const uchar *gChannelImageData = gChannelImage.ptr<uchar>(y);
            const uchar *maskImageData = mask.ptr<uchar>(y);
            for (int x = 0; x < colorImage.cols; ++x) {
                if (*maskImageData == 255) {
                    sumOfGreenValues += *gChannelImageData;
                    numberOfMaskPixel++;
                }
                maskImageData++;
                gChannelImageData++;
            }
        }
        auto gValue = static_cast<float>(sumOfGreenValues * 1.00 / numberOfMaskPixel * 1.00);
        cout << "gValue = " << gValue << endl;
    }

    double executionTime = (getTickCount() * 1.00 - prevTickCount * 1.00) / (getTickFrequency() * 1.00);
    cout << "execution Time = " << executionTime << " s" << endl;

    namedWindow("Color Image", WINDOW_NORMAL);
    imshow("Color Image", colorImage);
    //namedWindow("Threshold Image", WINDOW_NORMAL);
    //imshow("Threshold Image", thresholdImage);

    waitKey(0);
    return 0;
}