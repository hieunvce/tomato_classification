
#ifndef TOMATOCLASSIFICATION_FUNCTION_H
#define TOMATOCLASSIFICATION_FUNCTION_H

std::vector<cv::Point> removeDefectsFromContour(const std::vector<cv::Point> &contour,
                                         const std::vector<int> &defectsIndex, int &minDistance);
void calculateSize(cv::Mat& image, cv::Size2i& tomatoSize, cv::Mat& ROI);
std::vector<int> find3PeaksHistogram(cv::Mat& ROI);
int trainColorModel(std::string directory);
double predictColor(std::vector<int> feature);

#endif //TOMATOCLASSIFICATION_FUNCTION_H
