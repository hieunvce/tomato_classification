
#ifndef TOMATOCLASSIFICATION_FUNCTION_H
#define TOMATOCLASSIFICATION_FUNCTION_H

int distanceBetween2Points(const cv::Point &A, const cv::Point &B);
std::vector<cv::Point> removeFromContour(const std::vector<cv::Point> &contour,
                                         const std::vector<int> &defectsIndex, int &minDistance);
void calculateSize(cv::Mat& image, cv::Size2i& tomatoSize, cv::Mat& ROI);
std::vector<int> find3PeaksHistogram(cv::Mat& ROI);
double predictColor(std::vector<int> feature);

#endif //TOMATOCLASSIFICATION_FUNCTION_H
