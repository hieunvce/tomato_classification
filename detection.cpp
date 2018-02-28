//
// Created by henry on 2/26/18.
//

#include "detection.h"

RotatedRect detection(Mat srcImage, Mat segImage)
{
    Mat draw;
    cvtColor(segImage,segImage,CV_BGR2GRAY);
    Canny(segImage, segImage, 50, 150, 3);
    segImage.convertTo(draw,CV_8U);
    //namedWindow("image", CV_WINDOW_AUTOSIZE);
    //imshow("image", draw);
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    RNG rng(12345);
    findContours( draw, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
    /// Draw contours
    RotatedRect tomatoBox;
    Mat detectedImage = srcImage.clone();
    cout << "Contour size: " << contours.size() << endl;
    for( int i = 0; i< contours.size(); i++ )
    {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        //drawContours( detectedImage, contours, i, color, 2, 8, hierarchy, 0, Point() );
        tomatoBox = fitEllipse(contours[i]);
        Point2f vertices2f[4];
        tomatoBox.points(vertices2f);
        Point vertices[4];
        for (int i=0;i<4;++i) {
            vertices[i] = vertices2f[i];
        }
        Size axes;
        axes.height = tomatoBox.size.height/2;
        axes.width = tomatoBox.size.width/2;
        ellipse(detectedImage,tomatoBox.center,axes,tomatoBox.angle,0.0,360.0,color,2,8,0);

    }
    namedWindow("Result window",CV_WINDOW_AUTOSIZE);
    imshow( "Result window", detectedImage);
    return tomatoBox;
}


