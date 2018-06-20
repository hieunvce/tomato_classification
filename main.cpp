#include <iostream>
#include <opencv2/opencv.hpp>
#include "colorProfile.h"
#include "function.h"

using namespace std;
using namespace cv;

vector<Vec3b> setColor={Vec3b(0,0,255),Vec3b(0,255,255),
                        Vec3b(0,255,0),Vec3b(255,255,255)};

int main(int argc, char** argv) {
    VideoCapture camera("clip.mp4");//pass o to open webcame
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
        /*cout << "TOMATO GRADE: ";
        switch (status){
            case RED_NORMAL:
                cout << "RED_NORMAL"<< endl;
                break;
            case RED_BAD:
                cout << "RED_BAD"<< endl;
                break;
            case YELLOW_NORMAL:
                cout << "YELLOW_NORMAL"<< endl;
                break;
            case YELLOW_BAD:
                cout << "YELLOW_BAD"<< endl;
                break;
            case GREEN_NORMAL:
                cout << "GREEN_NORMAL" << endl;
                break;
            case GREEN_BAD:
                cout << "GREEN_BAD" << endl;
                break;
            case SKIP_SUCCESS:
                cout << "SKIP_SUCCESS" << endl;
                break;
            case FAIL:
                cout << "FAIL" << endl;
                exit(1);
        }*/
    }
    camera.release();
    video.release();
    destroyAllWindows();
    return 0;
}