//
// Created by hieunguyen on 6/14/18.
//

#include "function.h"

/**
 * findColor: Find main color of a tomato.
 * We count number of RED pixel, YELLOW pixel, GREEN pixel
 * then compare to find out main color.
 * Return: enum Color
 * @param LabImage
 * @return
 */
Color findColor(Mat LabImage){
    assert(LabImage.channels() == 3);

    int countRed=0;
    int countYellow=0;
    int countGreen=0;

    /**<Count number of pixel for each color*/
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {

            lab_data++; // dismiss L value
            int a = *lab_data++;
            a-=128;
            int b = *lab_data++;
            b-=128;

            if (isRed(a,b))
                countRed++;
            else if (isYellow(a,b))
                countYellow++;
            else if (isGreen(a,b))
                countGreen++;
        }
    }

    /**<Compare and return main color*/
    if (countRed>=countYellow && countRed>=countGreen) {
        return RED;
    }
    else if (countYellow>=countRed && countYellow>=countGreen) {
        return YELLOW;
    }
    else {
        return GREEN;
    }
}

/**
 * segmentImage: Segment image by color
 * Go through each pixel. If pixel's color is the same with color ID, it's value
 * at return image is 1,otherwise is 0.
 * Return: Mat
 * @param LabImage
 * @param colorID
 * @return
 */
Mat segmentImage(Mat LabImage, Color colorID){
    assert(LabImage.channels() == 3);
    Mat segImage(LabImage.rows,LabImage.cols,CV_8UC3);

    /**> Begin segmentation process */
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        uchar *seg_data = segImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            lab_data++;//dismiss L value
            int a = *lab_data++;
            a -= 128;
            int b = *lab_data++;
            b -= 128;
            if (color(a, b) == colorID) {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
            } else {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }

    int morph_size=2;
    Mat kernel = getStructuringElement(2,Size(2*morph_size + 1, 2*morph_size+1),Point(morph_size,morph_size));
    morphologyEx(segImage,segImage,MORPH_CLOSE,kernel);
    morphologyEx(segImage,segImage,MORPH_OPEN,kernel);
    morphologyEx(segImage,segImage,MORPH_CLOSE,kernel);

    return segImage;
}

/**
 * detectROI: Detect ROI which have tomato in image
 * It returns a rotated rectangle that best fit with ROI.
 * Return RotatedRect
 * @param segImage
 * @return
 */
RotatedRect detectROI(Mat segImage){
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

    //Check if contour existence
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

    return tomatoBox;
}