/*
 * DataCenter.cpp
 *
 *  Created on: Dec 22, 2015
 *      Author: ye
 */

#include "DataCenter.h"

DataCenter::DataCenter()
{
	m_basicFilePath = "param/Basic.yaml";
	m_segmentationThresholdFilePath = "param/SegmentationThreshold.yaml";
	m_cameraCalibrationFilePath = "param/CameraCalibration.yaml";
	m_imageTransformFileNamePath = "param/ImageTransform.yaml";

	loadAllParam();
}

DataCenter::~DataCenter()
{

}

bool DataCenter::loadAllParam()
{
	bool result;
	if(loadBasicParam() && loadSegmentationThreshold()
			&& loadCameraCalibration() && loadImageTransform())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DataCenter::loadCameraCalibration()
{
	cv::FileStorage paramFile = cv::FileStorage(m_cameraCalibrationFilePath, cv::FileStorage::READ);

	if(!paramFile.isOpened())
	{
		std::cout << "No such file: " << m_cameraCalibrationFilePath << std::endl;
		return false;
	}

	paramFile["camera_matrix"] >> m_cameraMatrix;
	paramFile["distortion_coefficients"] >> m_distCoeffs;

	paramFile.release();
	return true;
}

bool DataCenter::loadSegmentationThreshold()
{
    cv::FileStorage paramFile = cv::FileStorage(m_segmentationThresholdFilePath, cv::FileStorage::READ);

	if(!paramFile.isOpened())
	{
		std::cout << "No such file: " << m_segmentationThresholdFilePath << std::endl;
		return false;
	}

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

bool DataCenter::saveSegmentationThreshold()
{
	cv::FileStorage paramFile = cv::FileStorage(m_segmentationThresholdFilePath, cv::FileStorage::WRITE);

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

bool DataCenter::loadImageTransform()
{
    cv::FileStorage paramFile = cv::FileStorage(m_imageTransformFileNamePath, cv::FileStorage::READ);

	if(!paramFile.isOpened())
	{
		std::cout << "No such file: " << m_imageTransformFileNamePath << std::endl;
		return false;
	}

    paramFile["mapVertex"] >> m_mapVertex;
    paramFile["transMatrix"] >> m_transMatrix;
    paramFile["imageCenterInMap"] >> m_imageCenterInMap;

    paramFile.release();
    return true;
}

bool DataCenter::saveImageTransform()
{
	cv::FileStorage paramFile = cv::FileStorage(m_imageTransformFileNamePath, cv::FileStorage::WRITE);

	paramFile << "mapVertex" << m_mapVertex;
	paramFile << "transMatrix" << m_transMatrix;
	paramFile << "imageCenterInMap" << m_imageCenterInMap;

	paramFile.release();
    return true;
}

bool DataCenter::loadBasicParam()
{
    cv::FileStorage paramFile = cv::FileStorage(m_basicFilePath, cv::FileStorage::READ);

	if(!paramFile.isOpened())
	{
		std::cout << "No such file: " << m_basicFilePath << std::endl;
		return false;
	}

    paramFile["CameraIndex"] >> m_cameraString;
    paramFile["CameraHeight"] >> m_cameraHeight;
    paramFile["ImageWidth"] >> m_imageWidth;
    paramFile["ImageHeight"] >> m_imageHeight;

    paramFile["FieldWidth"] >> m_fieldWidth;
    paramFile["FieldHeight"] >> m_fieldHeight;

    paramFile["RobotRadius"] >> m_robotRadius;
    paramFile["RobotHeight"] >> m_robotHeight;

    paramFile["NodeName"] >> m_mqttNodeName;
    paramFile["MasterIP"] >> m_mqttMasterIP;
    paramFile["LocalizationTopic"] >> m_mqttLocalizationTopic;

    paramFile.release();
    return true;
}

bool DataCenter::saveBasicParam()
{
	cv::FileStorage paramFile = cv::FileStorage(m_basicFilePath, cv::FileStorage::WRITE);

	/* Do nothing */

	paramFile.release();
    return true;
}
