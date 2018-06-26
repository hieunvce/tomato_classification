#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;

vector<Vec3b> setColor={Vec3b(0,0,255),Vec3b(0,255,255),
                        Vec3b(0,255,0),Vec3b(255,255,255)};

int main(int argc, char** argv) {
    /*VideoCapture camera("clip.mp4");//pass o to open webcame
    if (!camera.isOpened()){
        cout << "Error opening video/camera!" << endl;
        return -1;
    }

    // Default resolution of the frame is obtained.The default resolution is system dependent.
    int frame_width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
    int frame_height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);

    // Define the codec and create VideoWriter object.The output is stored in 'outcpp.avi' file.
    VideoWriter video("outcpp.avi",CV_FOURCC('M','J','P','G'),10, Size(frame_width,frame_height));
    while (1){
        Mat frame;
        camera >> frame;
        if (frame.empty())
            break;
        Mat outFrame=runOnImage(frame);
        video.write(outFrame);
    }
    camera.release();
    video.release();
    destroyAllWindows();
     */
    Mat srcImage=imread("images/samples/1.jpg",CV_LOAD_IMAGE_COLOR);
    Size standardSize(500,500);
    resize(srcImage,srcImage,standardSize);
    Mat LabImage(srcImage.rows, srcImage.cols, CV_8UC3);
    cvtColor(srcImage, LabImage, COLOR_BGR2Lab);
    Mat segImage(srcImage.rows, srcImage.cols, CV_8U);
    Color colorID;
    SegmentImagev2(LabImage,segImage,colorID);
    namedWindow("lab image",CV_WINDOW_AUTOSIZE);
    imshow("lab image",LabImage);
    namedWindow("seg image",CV_WINDOW_AUTOSIZE);
    imshow("seg image",segImage);
    namedWindow("src image",CV_WINDOW_AUTOSIZE);
    imshow("src image",srcImage);
    cout << "Color: " << colorID << endl;
    waitKey(0);

    return 0;
}