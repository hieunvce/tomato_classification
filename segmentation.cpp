//
// Created by henry on 2/24/18.
//
#include "segmentation.h"

void segmentation(Mat srcImage, Mat segImage)
{
    //============================================================
    //namedWindow("Source Image",CV_WINDOW_AUTOSIZE);
    //imshow("Source Image",srcImage);
    //============================================================

    // Segmentation
    assert(srcImage.channels() == 3);
    Mat medianImage(srcImage.rows,srcImage.cols,CV_8UC3);
    int count =0;
    Mat kernel;
    for (int i=0;i<srcImage.rows;++i) {
        const uchar *src_data = srcImage.ptr<uchar>(i);
        uchar *seg_data = segImage.ptr<uchar>(i);

        for (int j = 0; j < srcImage.cols; ++j) {

            unsigned int b = *src_data++;
            unsigned int g = *src_data++;
            unsigned int r = *src_data++;

            if ((r >= 100 && g < 100 && b < 100)) {
                count++;
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
    cout << "Count: " << count << endl;
}

