//
// Created by hieunguyen on 7/6/18.
//

#include "runProgram.h"

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
        //imshow("segment",segImage);
        if (colorID != OTHER) {
            vector<Point> ROI = {Point(0, 0)};
            ROI = detectROI(segImage);

            Size2i tomatoSize;
            tomatoSize = calculateSize(ROI);

            Size sizeOfMask(srcImage.cols,srcImage.rows);
            Mat maskImage = createMask(sizeOfMask, ROI);
            int badPixels = 0;
            badPixels = countBadPixel(LabImage, maskImage);
            STATUS grade=gradeTomato(colorID, badPixels);
            showInfo(tomatoSize,grade);
            Scalar black_color = Scalar(0, 0, 0);
            polylines(srcImage, ROI, true, black_color, 2, 8);
            RotatedRect minRect=minAreaRect(ROI);
            Point2f rectPoints[4]; minRect.points( rectPoints );
            for( int j = 0; j < 4; j++ )
                line( srcImage, rectPoints[j], rectPoints[(j+1)%4], black_color, 2, 8 );
            string text=textGrade(grade);
            //text = text + " | " + to_string(tomatoSize.width) + " x " + to_string(tomatoSize.height);
            putText(srcImage,text,Point(10,20),FONT_HERSHEY_COMPLEX_SMALL,1,black_color,1);
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

    VideoWriter video("outcpp.avi", CV_FOURCC('M', 'J', 'P', 'G'), 24, Size(frame_width, frame_height));
    while (1) {
        Mat frame;
        camera >> frame;
        if (frame.empty())
            break;
        Mat outFrame = runOnImage(frame);
        video.write(outFrame);
        imshow("On Camera", outFrame);
        char c = (char) waitKey(10);
        if (c == 27)
            break;
    }
    camera.release();
    video.release();
    return 0;
}
