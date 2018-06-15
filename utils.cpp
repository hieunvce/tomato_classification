//
// Created by hieunguyen on 6/15/18.
//
#include "utils.h"
#include "colorProfile.h"

/*  //----------SHOW IMAGES FOR DEBUGGING-----------------------------------------------------

    namedWindow("lab image",WINDOW_AUTOSIZE);
    imshow("lab image",LabImage);
    cout << "LabImage: rows= " << LabImage.rows << "\t columns= " << LabImage.cols <<endl;

    ///for debug
    cout << "colorID=" << colorID << endl;
    namedWindow("red segment image", WINDOW_AUTOSIZE);
    imshow("red segment image", segImage);
    cout << "segImage: rows= " << segImage.rows << "\t columns= " << segImage.cols << endl;
    //end debug

    namedWindow("Mask Image", WINDOW_AUTOSIZE);
    imshow("Mask Image", maskImage);

    Scalar color = Scalar(0, 0, 0);
    polylines(srcImage,ROI,true,color,2,8);
    namedWindow("Detected Image", WINDOW_AUTOSIZE);
    imshow("Detected Image", srcImage);

    //--------END SHOW IMAGES FOR DEBUGGING --------------------------------------------------
 */

/*
   //------------file function.cpp | function countBadPixel | after segment bad region------
   //------------Can use to return vector <vector<Point> > badContours if necessary-----------
   //------------Add in future if neccessary--------------------------------------------------
   //------------FIND COUNTOURS OF BAD REGIONS------------------------------------------------
            ///for debug
            namedWindow("bad regions image", WINDOW_AUTOSIZE);
            imshow("bad regions image", badSegImage);
            cout << "Number of Bad pixel = " << countBadPixel << endl;
            //end debug
   // Find contours of bad regions
   // Find edge using Canny algorithm
   Mat edgeImage,kernel;
   cvtColor(badSegImage,badSegImage,CV_BGR2GRAY);
   Canny(badSegImage, badSegImage, 0, 255, 3);
   morphologyEx(badSegImage,badSegImage,MORPH_CLOSE,kernel);
   badSegImage.convertTo(edgeImage,CV_8U);

   //Find contour base on edge
   vector<vector<Point> > contours;
   vector<Vec4i> hierarchy;
   findContours(edgeImage, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

   //Check if contour existence
   if (contours.size()==0)
   {
       exit(-1);
   }
   //---------------END FIND COUNTOURS OF BAD REGIONS-------------------------------------------
   */