//
// Created by henry on 2/24/18.
//
#include "segmentation.h"

Mat segmentation(Mat srcImage)
{
    Mat LabImage(srcImage.rows,srcImage.cols,CV_8UC3);
    cvtColor(srcImage,LabImage,COLOR_BGR2Lab);
    Mat segImage(srcImage.rows,srcImage.cols,CV_8UC3);
    // Segmentation
    assert(LabImage.channels() == 3);
    Mat medianImage(LabImage.rows,LabImage.cols,CV_8UC3);
    Mat kernel;
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        uchar *seg_data = segImage.ptr<uchar>(i);

        for (int j = 0; j < LabImage.cols; ++j) {

            unsigned int l = *lab_data++;
            l=l*100/255;
            unsigned int a = *lab_data++;
            if (a>=128)
                a-=128;
            else
                a=0;
            unsigned int b = *lab_data++;
            if (b>=128)
                b-=128;
            else
                b=0;
            cout << "Lab = " << l << " " << a << " " << b << endl;
            if ((a>=20) && (a<=75) && (b>=20) && (b<=65) && abs(a-b)>=0 && abs(a-b)<=21) {
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
    morphologyEx(segImage,segImage,MORPH_CLOSE,kernel,Point(1,1),2);
    morphologyEx(segImage,segImage,MORPH_OPEN,kernel);
    //**************************************************************
    //Just for debuging
    namedWindow("SegImage",CV_WINDOW_AUTOSIZE);
    imshow("SegImage",segImage);
    //**************************************************************
    return segImage;
}

