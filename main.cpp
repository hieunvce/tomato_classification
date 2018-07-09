#include <iostream>
#include <string.h>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "runProgram.h"

using namespace std;
using namespace cv;

vector<Vec3b> setColor={Vec3b(0,0,255),Vec3b(0,255,255),
                        Vec3b(0,255,0),Vec3b(255,255,255)};

int main(int argc, char** argv) {
    Mat srcImage=imread(argv[1]);
    Mat LabImage;
    cvtColor(srcImage,LabImage,CV_BGR2Lab);
    //Mat result = runOnImage(srcImage);
    //imshow("result",result);

    /**> Begin segmentation process */
    for (int i=0;i<LabImage.rows;++i) {
        const uchar *lab_data = LabImage.ptr<uchar>(i);
        for (int j = 0; j < LabImage.cols; ++j) {
            int l = *lab_data++;
            l = l * 100 / 255;
            int a = *lab_data++ - 128;
            int b = *lab_data++ - 128;
            cout << l << "\t" << a << "\t"<< b << endl;
        }
    }
            //waitKey(0);
    return 0;
}