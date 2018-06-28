//
// Created by hieunguyen on 6/14/18.
//

#include "function.h"

#define MIN_NUMBER_PIXEL 20

/**
 * Segment LabImage  and find main color of tomato, return at colorID
 * Return: void
 *
 * @param LabImage
 * @param segImage: Result of segmentation process
 * @param colorID: Main color of tomato
 */
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
                case YELLOW:
                    countYellowPixel++;
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

/**
 * detectROI: Detect ROI which have tomato in image
 * It returns a contour is the ROI.
 * Return vector<Point>
 * @param segImage
 * @return
 */
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

/**
 * showInfo: Print to console info about tomato like color, size, number of bad pixels
 * @param tomatoColor
 * @param sizeOfTomato
 * @param nOfBadPixels
 */
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

/**
 * runOnImage: Run all function on an image
 * -----------------------------------------------------------------------------
 * segmentImage -> detecROI -> calculateSize
 * -> createMask -> countBadPixel -> gradeTomato
 * -----------------------------------------------------------------------------
 * Return: grade of tomato or status of function
 * @param srcImage
 * @return
 */
Mat runOnImage(Mat srcImage){
    try {
        Mat LabImage(srcImage.rows, srcImage.cols, CV_8UC3);
        cvtColor(srcImage, LabImage, COLOR_BGR2Lab);
        Mat segImage(srcImage.rows, srcImage.cols, CV_8U);
        Color colorID;
        SegmentImage(LabImage,segImage,colorID);
        imshow("segmented",segImage);
        if (colorID != OTHER) {
            vector<Point> ROI = {Point(0, 0)};
            ROI = detectROI(segImage);

            Size2i tomatoSize;
            tomatoSize = calculateSize(ROI);

            Size sizeOfMask(srcImage.cols,srcImage.rows);
            Mat maskImage = createMask(sizeOfMask, ROI);
            imshow("mask",maskImage);
            int badPixels = 0;
            badPixels = countBadPixel(LabImage, maskImage);
            STATUS grade=gradeTomato(colorID, badPixels);
            showInfo(tomatoSize,grade);
            Scalar black_color = Scalar(0, 0, 0);
            polylines(srcImage, ROI, true, black_color, 2, 8);

            RotatedRect rec=fitEllipse(ROI);
            RotatedRect rec2=minAreaRect(ROI);

            ellipse(srcImage,rec,black_color,2,8);

            return srcImage;
        } else {
            cout << "Skipped because of 0 tomato detected..." << endl;
            return srcImage;
        }
    }
    catch (exception &e)
    {
        cout << "Standard exception: " << e.what() << endl;
        exit(-12);
    }
}

/**
 * Run program on camera or a video
 * To write video, pass writeVideo=1
 * @param camera
 * @param writeVideo
 * @return
 */
int runOnCamera(VideoCapture camera){
    if (!camera.isOpened()){
        cout << "Error opening video/camera!" << endl;
        return -1;
    }

    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

    VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 10, Size(frame_width, frame_height));
    while (1) {
        Mat frame;
        camera >> frame;
        if (frame.empty())
            break;
        Mat outFrame = runOnImage(frame);
        video.write(outFrame);
        imshow("On Camera", outFrame);
        char c = (char) waitKey(40);
        if (c == 27)
            break;
    }
    camera.release();
    video.release();
    return 0;
}
