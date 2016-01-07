/*
 * Image2World.h
 *
 *  Created on: Jan 6, 2016
 *      Author: ye
 */

#ifndef SRC_IMAGE2WORLD_H_
#define SRC_IMAGE2WORLD_H_

#include "opencv2/opencv.hpp"

class Image2World {
public:
	cv::Mat m_cameraMatrix;
	cv::Mat m_distCoeffs;
	cv::Mat m_transMat;
public:
	Image2World();
	~Image2World();

	bool undistortPoints(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB);
	bool getTransMat(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB, cv::InputOutputArray in_mat);
	bool perspectiveTransform(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB, cv::InputArray in_mat);
};

#endif /* SRC_IMAGE2WORLD_H_ */
