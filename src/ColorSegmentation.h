/*
 * ColorSegmentation.h
 *
 *  Created on: Dec 23, 2015
 *      Author: ye
 */

#ifndef SRC_COLORSEGMENTATION_H_
#define SRC_COLORSEGMENTATION_H_

#include <opencv2/opencv.hpp>

class ColorSegmentation {
private:
	cv::Scalar m_minValue;
	cv::Scalar m_maxValue;
public:
	cv::Mat m_yuvImage;
	cv::Mat m_thresholdImage;
	cv::Mat m_filterImage;

	std::vector<cv::Point2d> m_points;
	std::vector<std::vector<cv::Point> > m_contours;
public:
	ColorSegmentation();
	~ColorSegmentation();

	void drawPoints(cv::InputOutputArray in_image, cv::Scalar in_color);
	void drawContours(cv::InputOutputArray in_image, cv::Scalar in_color);

	bool setThreshold(cv::Scalar minValue, cv::Scalar maxValue);
	bool getBlocks(cv::InputArray in_image, std::vector<cv::Point2d>& in_points);
};

#endif /* SRC_COLORSEGMENTATION_H_ */
