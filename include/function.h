
#ifndef TOMATOCLASSIFICATION_FUNCTION_H
#define TOMATOCLASSIFICATION_FUNCTION_H

int distanceBetween2Points(const cv::Point &A, const cv::Point &B);
std::vector<cv::Point> removeFromContour(const std::vector<cv::Point> &contour,
                                         const std::vector<int> &defectsIndex, int &minDistance);
void calculateSizeAndGValue(const cv::Mat& ROI, cv::Size2i& tomatoSize, float& gValue);

#endif //TOMATOCLASSIFICATION_FUNCTION_H
