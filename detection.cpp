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

    //Fit an ellipse to detected contour
    RotatedRect tomatoBox;
    tomatoBox = fitEllipse(largestContour);
    Point2f vertices2f[4];
    tomatoBox.points(vertices2f);
    Point vertices[4];
    for (int i=0;i<4;++i) {
        vertices[i] = vertices2f[i];
    }
    Size axes;
    axes.height = tomatoBox.size.height/2;
    axes.width = tomatoBox.size.width/2;

    //Draw ellipse to srcImage
    Scalar color = Scalar(255,255,255);
    cout << "Tomato size: Height: " << axes.height << " | Width: " << axes.width << endl;
    ellipse(srcImage,tomatoBox.center,axes,tomatoBox.angle,0.0,360.0,color,2,8,0);

    cout << "Detection Done!" << endl;
    maxContourAreaReturn = maxContourArea;
    return tomatoBox;
}