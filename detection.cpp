//
// Created by henry on 2/26/18.
//

#include "detection.h"

RotatedRect detection(Mat srcImage, Mat segImage, float &maxContourAreaReturn)
{

    // Find edge using Canny algorithm
    Mat edgeImage,kernel;
    cvtColor(segImage,segImage,CV_BGR2GRAY);
    Canny(segImage, segImage, 0, 255, 3);
    morphologyEx(segImage,segImage,MORPH_CLOSE,kernel);
    segImage.convertTo(edgeImage,CV_8U);

    //Find contour base on edge
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(edgeImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //Check if contour existtence
    if (contours.size()==0)
    {
        RotatedRect tomatoBox(Point2f(2,2),Size2f(1,1),0);
        return tomatoBox;
    }

    //Convexthull contour
    vector<vector<Point> >hull(contours.size());
    for (size_t i=0;i<contours.size();i++)
    {
        convexHull(contours[i],hull[i],false);
    }

    // Find largest Contour => is Tomato
    vector<Point> largestContour;
    float maxContourArea=contourArea(hull[0]);
    largestContour=hull[0];
    for (int i=0;i<hull.size();i++) {
        float area = contourArea(hull[i]);
        if (area > maxContourArea) {
            maxContourArea = area;
            largestContour = hull[i];
        }
    }
    //Mat contourImage=srcImage.clone();
    //drawContours(contourImage,largestContour,(0,0,0),2,8);
    namedWindow("Edge Image",WINDOW_AUTOSIZE);
    imshow("Edge Image",contourImage);

    //Fit an ellipse to detected contour
    RotatedRect tomatoBox;
    tomatoBox = fitEllipse(largestContour);

    maxContourAreaReturn = maxContourArea;
    return tomatoBox;
}