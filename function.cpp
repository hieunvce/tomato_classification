

#include "function.h"
using namespace std;
using namespace cv;

#define MIN_NUMBER_PIXEL 20


void SegmentImage(Mat LabImage, Mat &segImage, Color &colorID){
    assert(LabImage.channels() == 3);

    int countRedPixel=0;
    int countYellowPixel=0;
    int countGreenPixel=0;

    /**> Begin segmentation process */
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        uchar *seg_data = segImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            int l = *lab_data++;
            l=l*100/255;
            int a = *lab_data++-128;
            int b = *lab_data++-128;

            Color pixelColor=color(l,a,b);
            switch (pixelColor){
                case RED:
                    countRedPixel++;
                    *seg_data++=255;
                    break;
                case YELLOW:
                    countYellowPixel++;
                    *seg_data++=255;
                    break;
                case GREEN:
                    countGreenPixel++;
                    *seg_data++=255;
                    break;
                default:
                    *seg_data++=0;
            }
        }
    }
    if (countRedPixel >= MIN_NUMBER_PIXEL || countYellowPixel >= MIN_NUMBER_PIXEL || countGreenPixel >= MIN_NUMBER_PIXEL) {
        if (countRedPixel >= countYellowPixel && countRedPixel >= countGreenPixel) {
            colorID=RED;
        } else if (countYellowPixel >= countRedPixel && countYellowPixel >= countGreenPixel) {
            colorID=YELLOW;
        } else {
            colorID=GREEN;
        }
    } else
        colorID=OTHER;
}


vector<Point> detectROI(Mat segImage){

    //Find contour base on segImage
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(segImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    //Check if contour existence
    if (contours.size()==0)
    {
        exit(-1);
    }

    // Find largest Contour => is Tomato
    vector<Point> largestContour;
    float maxContourArea=contourArea(contours[0]);
    largestContour=contours[0];
    for (int i=0;i<contours.size();i++) {
        float area = contourArea(contours[i]);
        if (area > maxContourArea) {
            maxContourArea = area;
            largestContour = contours[i];
        }
    }

    //Convexthull contour
    vector<Point> ROI;
    convexHull(largestContour,ROI);

    return ROI;
}

Size2i calculateSize(vector<Point> ROI){
    //Fit an rotated rectangle to ROI
    RotatedRect boundingBox;
    boundingBox = minAreaRect(ROI);
    Size2i axes;
    axes.height = (int)boundingBox.size.height / 2;
    axes.width = (int)boundingBox.size.width / 2;

    return axes;
}

Mat createMask(Size sizeOfMask, vector<Point> ROI){
    Mat mask=Mat::zeros(sizeOfMask,CV_8UC3);
    vector<vector<Point> > ROI_Array;
    // Push ROI to an array to pass as argument to fillPoly function
    ROI_Array.push_back(ROI);
    Scalar white = Scalar(255, 255, 255);
    fillPoly(mask,ROI_Array,white);
    return mask;
}


int countBadPixel(Mat LabImage, Mat maskImage) {
    cvtColor(maskImage, maskImage, COLOR_BGR2GRAY);
    int numberOfBadPixel = 0;
    for (int i = 0; i < LabImage.rows; ++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            int l = *lab_data++;
            l=l*100/255;
            int a = *lab_data++-128;
            int b = *lab_data++-128;
            if (maskImage.at<uchar>(i, j) == 255 && color(l,a, b) == OTHER) {
                numberOfBadPixel++;
            }
        }
    }
    return numberOfBadPixel;
}

STATUS gradeTomato(Color colorID, int nOfBadPixels){
    cout << " BAD PIXELS: " << nOfBadPixels << "\t|";

    int badTomato = 0;
    if (nOfBadPixels > MAX_BAD_PIXEL)
        badTomato = 1;
    switch (colorID) {
        case RED:
            if (badTomato)
                return RED_BAD;
            else
                return RED_NORMAL;
        case YELLOW:
            if (badTomato)
                return YELLOW_BAD;
            else
                return YELLOW_NORMAL;
        case GREEN:
            if (badTomato)
                return GREEN_BAD;
            else
                return GREEN_NORMAL;
        default:
            return SKIP_SUCCESS;
    }
}

void showInfo(Size2i sizeOfTomato, STATUS grade){
    switch (grade) {
        case RED_BAD:
            cout << "RED BAD \t|";
            break;
        case RED_NORMAL:
            cout << "RED NORMAL \t|";
            break;
        case YELLOW_BAD:
            cout << "YELLOW BAD \t|";
            break;
        case YELLOW_NORMAL:
            cout << "YELLOW NORMAL \t|";
            break;
        case GREEN_BAD:
            cout << "GREEN BAD \t|";
            break;
        case GREEN_NORMAL:
            cout << "GREEN NORMAL \t|";
            break;
        default:
            cout << "NO TOMATO \t|";
    }

    cout << " SIZE: " << sizeOfTomato.height << " x " << sizeOfTomato.width << endl;
}

string textGrade(STATUS grade){
    string text;
    switch (grade){
        case RED_BAD:
            return "R_B";
        case RED_NORMAL:
            return "R_N";
        case YELLOW_BAD:
            return "Y_B";
        case YELLOW_NORMAL:
            return "Y_N";
        case GREEN_BAD:
            return "G_B";
        case GREEN_NORMAL:
            return "G_N";
        default:
            return "";
    }
}

