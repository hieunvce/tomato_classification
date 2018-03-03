//
// Created by henry on 2/28/18.
//

#include "classifyByColor.h"
#include "colorProfile.h"

Mat createMaskImage(Mat srcImage, RotatedRect boundingBox)
{
    Mat maskImage = Mat::zeros(srcImage.rows,srcImage.cols,CV_8U);
    Scalar color = Scalar(255);
    ellipse(maskImage,boundingBox,color,-1);
    return maskImage;
}

void calculateEachColorPercentage(Mat srcImage, Mat maskImage)
{

    Mat LabImage(srcImage.rows,srcImage.cols,CV_8UC3);
    cvtColor(srcImage,LabImage,COLOR_BGR2Lab);

    assert(LabImage.channels() == 3);
    unsigned int count=0;
    unsigned int rCount=0;

    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            if(maskImage.at<unsigned char>(i,j)>0) {
                count++;
                lab_data++;
                int a = *lab_data++;
                a -= 128;
                int b = *lab_data++;
                b -= 128;
                if (isGreen(a,b)) {
                    rCount++;
                    maskImage.at<unsigned char>(i,j)=0;
                }
            } else{
                lab_data+=3;
            }
        }
    }

    float percentage = (float)rCount/count;
    percentage*=100;
    cout << "Red Pixel Percentage: " << percentage << "%" << endl;
}
