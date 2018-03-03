//
// Created by henry on 2/24/18.
//
#include "segmentation.h"
#include "colorProfile.h"

Mat segmentation(Mat srcImage)
{

    Mat LabImage(srcImage.rows,srcImage.cols,CV_8UC3);
    cvtColor(srcImage,LabImage,COLOR_BGR2Lab);
    Mat segImage(srcImage.rows,srcImage.cols,CV_8UC3);
    // Segmentation
    assert(LabImage.channels() == 3);

    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        uchar *seg_data = segImage.ptr<uchar>(i);

        for (int j = 0; j < LabImage.cols; ++j) {

            lab_data++;
            int a = *lab_data++;
            a-=128;
            int b = *lab_data++;
            b-=128;
            if (isGreen(a,b)) {
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

