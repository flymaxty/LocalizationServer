/*
 * Image2World.cpp
 *
 *  Created on: Jan 6, 2016
 *      Author: ye
 */

#include "Image2World.h"

Image2World::Image2World(DataCenter *in_dataCenter)
{
    m_cameraMatrix = in_dataCenter->m_cameraMatrix;
    m_distCoeffs = in_dataCenter->m_distCoeffs;

    m_transMat = in_dataCenter->m_transMatrix;

    m_fieldHeight = in_dataCenter->m_fieldHeight;
    m_fieldWidth = in_dataCenter->m_fieldWidth;
    m_imageHeight = in_dataCenter->m_imageHeight;
    m_imageWidth = in_dataCenter->m_imageWidth;

    m_cameraHeight = in_dataCenter->m_cameraHeight;
}

Image2World::~Image2World()
{
	// TODO Auto-generated destructor stub
}


bool Image2World::fixErrorByHeight(std::vector<cv::Point2d>& in_points, double in_ObjectHeight)
{
	double T = 1.0 - in_ObjectHeight / m_cameraHeight;
	cv::Point2d fieldCenter(m_fieldWidth / 2, m_fieldHeight / 2);

	for(uint16_t i=0; i< in_points.size(); i++)
	{
		std::cout << in_points[i] << " : ";
		in_points[i] = in_points[i] * T + fieldCenter*(1-T);
		std::cout << in_points[i] << std::endl;
	}
	return true;
}

bool Image2World::convert2Field(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB, double in_Objectheight)
{
	in_pointsB.clear();
	if(in_pointsA.size() == 0)
	{
		return true;
	}

	std::vector<cv::Point2d> undistortPointsB, mirrorFieldPoints;

	undistortPoints(in_pointsA, undistortPointsB);
	perspectiveTransform(undistortPointsB, in_pointsB);
	fixErrorByHeight(in_pointsB, in_Objectheight);
	//changeCoordinate(mirrorFieldPoints, in_pointsB);
	return true;
}

bool Image2World::changeCoordinate(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB)
{
	cv::Point2d tempPoints;
	double wScale = m_fieldWidth / m_imageWidth;
	double hScale = m_fieldHeight / m_imageHeight;

	in_pointsB.clear();

	/*for(uint16_t i; i<in_pointsA.size(); i++)
	{
		tempPoints.x = in_pointsA[i].x * wScale;
		tempPoints.y = m_fieldHeight - in_pointsA[i].y * hScale;
		in_pointsB.push_back(tempPoints);
	}*/

	for(uint16_t i; i<in_pointsA.size(); i++)
	{
		tempPoints.x = in_pointsA[i].x;
		tempPoints.y = in_pointsA[i].y;
		in_pointsB.push_back(tempPoints);
	}

	return true;
}

bool Image2World::undistortPoints(std::vector<cv::Point2d>& in_pointsA,
		std::vector<cv::Point2d>& in_pointsB)
{
	in_pointsB.clear();
	if(in_pointsA.size() == 0)
	{
		return true;
	}

	cv::Mat src(in_pointsA), dst;

	cv::undistortPoints(src, dst, m_cameraMatrix, m_distCoeffs, cv::noArray(), m_cameraMatrix);
	in_pointsB.clear();
	cv::Point2d tempPoint;
    for(int i=0; i<dst.rows; i++)
    {
    	std::cout << "x: " << dst.at<double>(i, 0) << " y: " << dst.at<double>(i, 1) << std::endl;
    	tempPoint.x = dst.at<double>(i, 0);
    	tempPoint.y = dst.at<double>(i, 1);
    	//tempPoint.x = dst.at<double>(i, 0) * m_cameraMatrix.at<double>(0, 0) + m_cameraMatrix.at<double>(0, 2);
    	//tempPoint.y = dst.at<double>(i, 1) * m_cameraMatrix.at<double>(1, 1) + m_cameraMatrix.at<double>(1, 2);
    	in_pointsB.push_back(tempPoint);
    }
	return true;
}

bool Image2World::getTransMat(cv::InputArray in_pointsA,
		cv::InputOutputArray in_pointsB, cv::OutputArray in_mat)
{
	in_mat.create(3, 3, CV_64F);
	cv::Mat transMat = in_mat.getMat();

	cv::Mat matA(*static_cast<const std::vector<cv::Point2d>*>(in_pointsA.getObj()));
	cv::Mat matB(*static_cast<const std::vector<cv::Point2d>*>(in_pointsB.getObj()));
	matA.convertTo(matA, CV_32FC2);
	matB.convertTo(matB, CV_32FC2);

	cv::Mat tempMat;
	tempMat = cv::getPerspectiveTransform(matA, matB);
	tempMat.copyTo(transMat);

	return true;
}

bool Image2World::perspectiveTransform(cv::InputArray in_pointsA,
		cv::InputOutputArray in_pointsB)
{
	in_pointsB.clear();

	if(in_pointsA.empty())
	{
		return true;
	}

	cv::perspectiveTransform(in_pointsA, in_pointsB, m_transMat);

	return true;
}
