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

#ifndef SRC_IMAGE2WORLD_H_
#define SRC_IMAGE2WORLD_H_

#include "opencv2/opencv.hpp"

#include "DataCenter.h"

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

    double m_cameraHeight;

public:
	Image2World(DataCenter *in_dataCenter);
	~Image2World();

	bool fixErrorByHeight(std::vector<cv::Point2d>& in_points, double in_ObjectHeight);
	bool undistortPoints(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB);
	bool getTransMat(cv::InputArray in_pointsA,
			cv::InputOutputArray in_pointsB, cv::OutputArray in_mat);
	bool perspectiveTransform(cv::InputArray in_pointsA,
			cv::InputOutputArray in_pointsB);
	bool convert2Field(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB, double in_ObjectHeight);
	bool changeCoordinate(std::vector<cv::Point2d>& in_pointsA,
			std::vector<cv::Point2d>& in_pointsB);
};

#endif /* SRC_IMAGE2WORLD_H_ */
