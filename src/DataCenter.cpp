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

}

DataCenter::~DataCenter()
{

}

bool DataCenter::loadParam()
{
	m_thresholdFile = cv::FileStorage(s_thresholdFileName, cv::FileStorage::READ);

	m_thresholdFile["TeamAMin"] >> m_teamAMin;
	m_thresholdFile["TeamAMax"] >> m_teamAMax;
	m_thresholdFile["TeamBMin"] >> m_teamBMin;
	m_thresholdFile["TeamBMax"] >> m_teamBMax;
	m_thresholdFile["TeamNumb1Min"] >> m_teamNumb1Min;
	m_thresholdFile["TeamNumb1Max"] >> m_teamNumb1Max;
	m_thresholdFile["TeamNumb2Min"] >> m_teamNumb2Min;
	m_thresholdFile["TeamNumb2Max"] >> m_teamNumb2Max;

	m_thresholdFile.release();
	return true;
}

bool DataCenter::saveParam()
{
	m_thresholdFile = cv::FileStorage(s_thresholdFileName, cv::FileStorage::WRITE);

	m_thresholdFile << "TeamAMin" << m_teamAMin;
	m_thresholdFile << "TeamAMax" << m_teamAMax;
	m_thresholdFile << "TeamBMin" << m_teamBMin;
	m_thresholdFile << "TeamBMax" << m_teamBMax;
	m_thresholdFile << "TeamNumb1Min" << m_teamNumb1Min;
	m_thresholdFile << "TeamNumb1Max" << m_teamNumb1Max;
	m_thresholdFile << "TeamNumb2Min" << m_teamNumb2Min;
	m_thresholdFile << "TeamNumb2Max" << m_teamNumb2Max;

	m_thresholdFile.release();
	return true;
}
