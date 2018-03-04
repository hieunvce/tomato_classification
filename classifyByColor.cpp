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
    unsigned int rCount,yCount,gCount,oCount;
    rCount=yCount=gCount=oCount=0;

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
                if (isRed(a,b)) {
                    rCount++;
                    maskImage.at<unsigned char>(i,j)=0;
                } else if (isYellow(a,b)){
                    yCount++;
                    maskImage.at<unsigned char>(i,j)=0;
                } else if (isGreen(a,b)){
                    gCount++;
                    maskImage.at<unsigned char>(i,j)=0;
                } else{
                    oCount++;
                }
            } else{
                lab_data+=3;
            }
        }
    }

    float redPercentage = (float)rCount/count;
    redPercentage*=100;
    float yellowPercentage = (float)yCount/count;
    yellowPercentage*=100;
    float greenPercentage = (float)gCount/count;
    greenPercentage*=100;
    float otherPercentage = (float)oCount/count;
    otherPercentage*=100;
    cout << endl;
    cout << "Red Pixel Percentage: " << redPercentage << "%" << endl;
    cout << "Yellow Pixel Percentage: " << yellowPercentage << "%" << endl;
    cout << "Green Pixel Percentage: " << greenPercentage << "%" << endl;
    cout << "Other color Pixel Percentage: " << otherPercentage << "%" << endl;
}
