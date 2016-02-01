/*
 * DataCenter.cpp
 *
 *  Created on: Dec 22, 2015
 *      Author: ye
 */

#include "DataCenter.h"

DataCenter::DataCenter()
{
	m_thresholdFileName = "Threshold.yaml";
	m_cameraMatrixFileName = "CameraMatrix.yaml";
	m_matrixFileName = "Matrix.yaml";

    m_fieldWidth = 0.67;
    m_fieldHeight = 0.57;

    m_robotRadius = 0.15;
}

DataCenter::~DataCenter()
{

}

bool DataCenter::loadAllParam()
{
	loadThreshold();
	loadCameraMatrix();
	loadMatrix();
}

bool DataCenter::loadCameraMatrix()
{
	cv::FileStorage paramFile = cv::FileStorage(m_cameraMatrixFileName, cv::FileStorage::READ);

	paramFile["camera_matrix"] >> m_cameraMatrix;
	paramFile["distortion_coefficients"] >> m_distCoeffs;

	paramFile.release();
	return true;
}

bool DataCenter::loadThreshold()
{
    cv::FileStorage paramFile = cv::FileStorage(m_thresholdFileName, cv::FileStorage::READ);

    paramFile["TeamAMin"] >> m_teamAMin;
    paramFile["TeamAMax"] >> m_teamAMax;
    paramFile["TeamBMin"] >> m_teamBMin;
    paramFile["TeamBMax"] >> m_teamBMax;
    paramFile["TeamNumb1Min"] >> m_teamNumb1Min;
    paramFile["TeamNumb1Max"] >> m_teamNumb1Max;
    paramFile["TeamNumb2Min"] >> m_teamNumb2Min;
    paramFile["TeamNumb2Max"] >> m_teamNumb2Max;
    paramFile["CartesianMin"] >> m_cartesianMin;
    paramFile["CartesianMax"] >> m_cartesianMax;

    paramFile.release();
    return true;
}

bool DataCenter::saveThreshold()
{
	cv::FileStorage paramFile = cv::FileStorage(m_thresholdFileName, cv::FileStorage::WRITE);

	paramFile << "TeamAMin" << m_teamAMin;
	paramFile << "TeamAMax" << m_teamAMax;
	paramFile << "TeamBMin" << m_teamBMin;
	paramFile << "TeamBMax" << m_teamBMax;
	paramFile << "TeamNumb1Min" << m_teamNumb1Min;
	paramFile << "TeamNumb1Max" << m_teamNumb1Max;
	paramFile << "TeamNumb2Min" << m_teamNumb2Min;
	paramFile << "TeamNumb2Max" << m_teamNumb2Max;
	paramFile << "CartesianMin" << m_cartesianMin;
	paramFile << "CartesianMax" << m_cartesianMax;

	paramFile.release();
    return true;
}

bool DataCenter::loadMatrix()
{
    cv::FileStorage paramFile = cv::FileStorage(m_matrixFileName, cv::FileStorage::READ);

    paramFile["mapVertex"] >> m_mapVertex;
    paramFile["transMatrix"] >> m_transMatrix;

    paramFile.release();
    return true;
}

bool DataCenter::saveMatrix()
{
	cv::FileStorage paramFile = cv::FileStorage(m_matrixFileName, cv::FileStorage::WRITE);

	paramFile << "mapVertex" << m_mapVertex;
	paramFile << "transMatrix" << m_transMatrix;

	paramFile.release();
    return true;
}
