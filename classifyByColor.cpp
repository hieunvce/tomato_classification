//
// Created by henry on 2/28/18.
//

#include "classifyByColor.h"

Mat createMaskImage(Mat srcImage, RotatedRect boundingBox)
{
    Mat maskImage = Mat::zeros(srcImage.rows,srcImage.cols,CV_8U);
    Scalar color = Scalar(255);
    ellipse(maskImage,boundingBox,color,-1);
    namedWindow("Mask Image", CV_WINDOW_AUTOSIZE);
    imshow("Mask Image",maskImage);
    return maskImage;
}

void calculateEachColorPercentage(Mat srcImage, Mat maskImage)
{
    Mat test = maskImage.clone();
    unsigned int image_height = srcImage.rows;
    unsigned int image_width = srcImage.cols;
    unsigned int count=0;
    unsigned int rCount=0;
    for (unsigned int y=0;y<image_height;++y)
        for (unsigned int x=0;x<image_width;++x)
            if(maskImage.at<unsigned char>(y,x)>0)
            {
                count++;
                const uchar *src_data = srcImage.ptr<uchar>(y,x);
                unsigned int b = *src_data++;
                unsigned int g = *src_data++;
                unsigned int r = *src_data;
                if ((r >= 100 && g < 100 && b < 100)) {
                    rCount++;
                    test.at<unsigned char>(y,x)=0;
                }
            }
    cout << "Count Mask Image/RCount: " << count << "/" << rCount << endl;
    float percentage = (float)rCount/count;
    percentage*=100;
    cout << "Red Pixel Percentage: " << percentage << "%" << endl;
    namedWindow("test",CV_WINDOW_AUTOSIZE);
    imshow("test",test);
}
