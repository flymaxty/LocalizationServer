/*
 * DataCenter.h
 *
 *  Created on: Dec 22, 2015
 *      Author: ye
 */

#ifndef SRC_DATACENTER_H_
#define SRC_DATACENTER_H_

#include <opencv2/opencv.hpp>

class DataCenter
{
public:
	//Robot Struct
	struct Robot
	{
		uint8_t id;
		bool online;
		double x;
		double y;
		float theta;
	};

	//Team Struct
	struct Team
	{
		Robot robots[2];
	};

	Team m_teamRed;
	Team m_teamGreen;

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
