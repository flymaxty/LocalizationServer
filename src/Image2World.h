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

    //Filed(Meter)
    double m_fieldWidth;
    double m_fieldHeight;

    //Image
    uint16_t m_imageWidth;
    uint16_t m_imageHeight;

public:
	Image2World();
	~Image2World();

	bool undistortPoints(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB);
	bool getTransMat(cv::InputArray in_pointsA,
			cv::InputOutputArray in_pointsB, cv::OutputArray in_mat);
	bool perspectiveTransform(cv::InputArray in_pointsA,
			cv::InputOutputArray in_pointsB);
	bool convert2Field(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB);
	bool changeCoordinate(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB);
};

#endif /* SRC_IMAGE2WORLD_H_ */
