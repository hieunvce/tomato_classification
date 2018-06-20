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
            int l = *lab_data++;
            l=l*100/255;
            int a = *lab_data++-128;
            int b = *lab_data++-128;

            if (isRed(l,a,b))
                countRed++;
            else if (isYellow(l,a,b))
                countYellow++;
            else if (isGreen(l,a,b))
                countGreen++;
        }
    }
    cout << countRed << ", " << countYellow << ", " << countGreen << endl;
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
            int l = *lab_data++;
            l=l*100/255;
            int a = *lab_data++-128;
            int b = *lab_data++-128;
            //for debugging
                    //cout << "(" << a << ", " << b << ") | sqrt(a^2+b^2)= " << sqrt(a*a+b*b) << endl;
                            //cout << sqrt(a*a+b*b) << endl;
            if (color(l,a, b) == colorID) {
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

/**
 * gradeTomato: Grade Tomato base on color and number of bad pixels
 * if (number of pixel > MAX_BAD_PIXEL) then tomato is bad.
 * Return: STATUS is grade of tomato
 * @param colorID
 * @param nOfBadPixels
 * @return
 */
STATUS gradeTomato(Color colorID, int nOfBadPixels){
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

/**
 * showInfo: Print to console info about tomato like color, size, number of bad pixels
 * @param tomatoColor
 * @param sizeOfTomato
 * @param nOfBadPixels
 */
void showInfo(Color tomatoColor, Size2i sizeOfTomato, int nOfBadPixels){
    switch (tomatoColor) {
        case RED:
            cout << "RED TOMATO \t|";
            break;
        case YELLOW:
            cout << "YELLOW TOMATO \t|";
            break;
        case GREEN:
            cout << "GREEN TOMATO \t|";
            break;
    }

    cout << " SIZE: " << sizeOfTomato.height << " x " << sizeOfTomato.width << "\t|";
    cout << " BAD PIXELS: " << nOfBadPixels << endl;

}

/**
 * runOnImage: Run all function on an image
 * -----------------------------------------------------------------------------
 * findColor -> segmentImage -> detecROI -> calculateSize
 * -> createMask -> countBadPixel -> gradeTomato
 * -----------------------------------------------------------------------------
 * Return: grade of tomato or status of function
 * @param srcImage
 * @return
 */
STATUS runOnImage(Mat srcImage){
    try {
        Size standardSize(500,500);
        resize(srcImage,srcImage,standardSize);
        Mat LabImage(srcImage.rows, srcImage.cols, CV_8UC3);
        cvtColor(srcImage, LabImage, COLOR_BGR2Lab);

        Color colorID = findColor(LabImage);

        if (colorID != OTHER) {
            Mat segImage(srcImage.rows, srcImage.cols, CV_8UC3);
            segImage = segmentImage(LabImage, colorID);

            vector<Point> ROI = {Point(0, 0)};
            ROI = detectROI(segImage);

            Size2i tomatoSize;
            tomatoSize = calculateSize(ROI);

            Size sizeOfMask;
            sizeOfMask.width = srcImage.cols;
            sizeOfMask.height = srcImage.rows;
            Mat maskImage = createMask(sizeOfMask, ROI);

            int badPixels = 0;
            badPixels = countBadPixel(LabImage, maskImage);
                    //----------SHOW IMAGES FOR DEBUGGING-----------------------------------------------------
                    ///for debug
                    cout << "colorID=" << colorID << endl;
                    namedWindow("segment image", WINDOW_AUTOSIZE);
                    imshow("segment image", segImage);
                    //end debug
                    //--------END SHOW IMAGES FOR DEBUGGING --------------------------------------------------
            showInfo(colorID, tomatoSize, badPixels);
            //-------Show image after detect-----------------------------------------
            Scalar black_color = Scalar(0, 0, 0);
            polylines(srcImage, ROI, true, black_color, 2, 8);
            namedWindow("Image", WINDOW_AUTOSIZE);
            imshow("Image", srcImage);
            //------End show image after detect--------------------------------------
            return gradeTomato(colorID,badPixels);


        } else {
            cout << "Skipped because of 0 tomato detected..." << endl;
            return SKIP_SUCCESS;
        }
    }
    catch (exception &e)
    {
        cout << "Standard exception: " << e.what() << endl;
        return FAIL;
    }
}