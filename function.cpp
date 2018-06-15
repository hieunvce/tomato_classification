//
// Created by hieunguyen on 6/14/18.
//

#include "function.h"

#define MIN_NUMBER_PIXEL 20
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
    if (countRed >= MIN_NUMBER_PIXEL || countYellow >= MIN_NUMBER_PIXEL || countGreen >= MIN_NUMBER_PIXEL) {
        if (countRed >= countYellow && countRed >= countGreen) {
            return RED;
        } else if (countYellow >= countRed && countYellow >= countGreen) {
            return YELLOW;
        } else {
            return GREEN;
        }
    } else {
        return OTHER;
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
 * It returns a contour is the ROI.
 * Return vector<Point>
 * @param segImage
 * @return
 */
vector<Point> detectROI(Mat segImage){
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
        exit(-1);
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

    return largestContour;
}

/**
 * calculateSize: calculate size of tomato (size of ROI)
 * Print size to console
 * Return: Size2i
 * @param ROI
 */
Size2i calculateSize(vector<Point> ROI){
    //Fit an ellipse to ROI
    RotatedRect boundingBox;
    boundingBox = fitEllipse(ROI);
    //Calculate ellipse from rotated rectangle
    Size2i axes;
    axes.height = (int)boundingBox.size.height / 2;
    axes.width = (int)boundingBox.size.width / 2;

    return axes;
}

/**
 * createMask: Create a mask image that fill ROI with 255,255,255
 * Return: Mat image
 * @param sizeOfMask
 * @param ROI
 * @return
 */
Mat createMask(Size sizeOfMask, vector<Point> ROI){
    Mat mask=Mat::zeros(sizeOfMask,CV_8UC3);
    vector<vector<Point> > ROI_Array;
    // Push ROI to an array to pass as argument to fillPoly function
    ROI_Array.push_back(ROI);
    Scalar white = Scalar(255, 255, 255);
    fillPoly(mask,ROI_Array,white);
    return mask;
}

/**
 * countBadPixel: Count number of bad pixels on tomato
 * + Get LabImage[pixel]
 * + If (maskImage[pixel]==1 && color[pixel]=OTHER) => badSegImage[pixel]=1, countBadPixel++, else =0
 * + Return: int numberOfBadPixels
 * + In future:
 * +            + Find contours of badSegImage
 * +            + Return: vector<vector<Point> > contains Bad contours
 * @return
 */
int countBadPixel(Mat LabImage, Mat maskImage){
    cvtColor(maskImage,maskImage,COLOR_BGR2GRAY);
    int numberOfBadPixel=0;
    // Segment Lab image to contain only Bad Region
    Mat badSegImage(LabImage.rows, LabImage.cols,CV_8UC3);
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        uchar *seg_data = badSegImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            lab_data++;//dismiss L value
            int a = *lab_data++;
            a -= 128;
            int b = *lab_data++;
            b -= 128;
            if (maskImage.at<uchar>(i,j)==255 && color(a, b) == OTHER) {
                *seg_data++ = 255;
                *seg_data++ = 255;
                *seg_data++ = 255;
                numberOfBadPixel++;
            } else {
                *seg_data++ = 0;
                *seg_data++ = 0;
                *seg_data++ = 0;
            }
        }
    }
    return numberOfBadPixel;
}

int runOnImage(Mat image){

}