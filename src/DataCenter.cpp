/*
 * DataCenter.cpp
 *
 *  Created on: Dec 22, 2015
 *      Author: ye
 */

#include "DataCenter.h"

const std::string DataCenter::s_thresholdFileName = "threshold.yaml";

DataCenter::DataCenter()
{
	// TODO Auto-generated constructor stub

}

DataCenter::~DataCenter()
{
	//TODO Auto-generated constructor stub
}

bool DataCenter::loadParam()
{
	m_thresholdFile = cv::FileStorage(s_thresholdFileName, cv::FileStorage::READ);

	m_thresholdFile["TeamRedMin"] >> m_teamRedMin;
	m_thresholdFile["TeamRedMax"] >> m_teamRedMax;
	m_thresholdFile["TeamGreenMin"] >> m_teamGreenMin;
	m_thresholdFile["TeamGreenMax"] >> m_teamGreenMax;
	m_thresholdFile["TeamNumb1Min"] >> m_teamNumb1Min;
	m_thresholdFile["TeamNumb1Max"] >> m_teamNumb1Max;
	m_thresholdFile["TeamNumb2Min"] >> m_teamNumb2Min;
	m_thresholdFile["TeamNumb2Max"] >> m_teamNumb2Max;

	m_thresholdFile.release();
	return true;
}
