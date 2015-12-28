/*
 * DataCenter.h
 *
 *  Created on: Dec 22, 2015
 *      Author: ye
 */

#ifndef SRC_DATACENTER_H_
#define SRC_DATACENTER_H_

#include <opencv2/opencv.hpp>

#include "RobotStruct.h"

class DataCenter
{
public:
	RobotTeam m_teamRed;
	RobotTeam m_teamGreen;

	//Threshold
	cv::Scalar m_teamRedMin;
	cv::Scalar m_teamRedMax;
	cv::Scalar m_teamGreenMin;
	cv::Scalar m_teamGreenMax;
	cv::Scalar m_teamNumb1Min;
	cv::Scalar m_teamNumb1Max;
	cv::Scalar m_teamNumb2Min;
	cv::Scalar m_teamNumb2Max;

	//Image
	cv::Mat m_rawImage;

private:
	static const std::string s_thresholdFileName;		//

	cv::FileStorage m_thresholdFile;					//
public:
	DataCenter();
	~DataCenter();

	bool loadParam();
private:
};

#endif /* SRC_DATACENTER_H_ */
