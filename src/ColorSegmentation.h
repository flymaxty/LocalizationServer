/**
* The MIT License (MIT)
* 
* Copyright (c) 2016 Tian Ye
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
**/

#ifndef SRC_COLORSEGMENTATION_H_
#define SRC_COLORSEGMENTATION_H_

#include <opencv2/opencv.hpp>

class ColorSegmentation {
private:
    cv::Scalar m_minValue;
    cv::Scalar m_maxValue;
    double m_minArea;
public:
    cv::Mat m_yuvImage;
    cv::Mat m_thresholdImage;
    cv::Mat m_filterImage;

    std::vector<cv::Point2d> m_points;
    std::vector<std::vector<cv::Point> > m_contours;
public:
    ColorSegmentation(cv::Scalar minValue, cv::Scalar maxValue, double in_minArea);
    ~ColorSegmentation();

    void drawPoints(cv::InputOutputArray in_image, cv::Scalar in_color);
    void drawContours(cv::InputOutputArray in_image, cv::Scalar in_color);

    bool setThreshold(cv::Scalar minValue, cv::Scalar maxValue);
    bool getBlocks(cv::InputArray in_image, std::vector<cv::Point2d>& in_points);
};

#endif /* SRC_COLORSEGMENTATION_H_ */
