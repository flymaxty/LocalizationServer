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
    RobotTeam m_teamA;
    RobotTeam m_teamB;

    //Threshold
    cv::Scalar m_teamAMin;
    cv::Scalar m_teamAMax;
    cv::Scalar m_teamBMin;
    cv::Scalar m_teamBMax;
    cv::Scalar m_teamNumb1Min;
    cv::Scalar m_teamNumb1Max;
    cv::Scalar m_teamNumb2Min;
    cv::Scalar m_teamNumb2Max;
    cv::Scalar m_cartesianMin;
    cv::Scalar m_cartesianMax;

    //Camera
    std::string m_cameraString;
    uint16_t m_imageWidth;
    uint16_t m_imageHeight;
    cv::Mat m_cameraMatrix;
    cv::Mat m_distCoeffs;

    //Map
    cv::Mat m_transMatrix;
    std::vector<cv::Point2d> m_mapVertex;

    //Filed
    double m_fieldWidth;
    double m_fieldHeight;

    //Robot
    double m_robotRadius;
    double m_robotHeight;

    //MQTT topic
    std::string m_mqttNodeName;
    std::string m_mqttMasterIP;
    std::string m_mqttLocalizationTopic;

public:
    std::string m_segmentationThresholdFilePath;
    std::string m_cameraCalibrationFilePath;
    std::string m_imageTransformFileNamePath;
    std::string m_basicFilePath;
public:
    DataCenter();
    ~DataCenter();

    bool loadAllParam();

    bool loadSegmentationThreshold();
    bool saveSegmentationThreshold();

    bool loadCameraCalibration();
    bool saveCameraCalibration();

    bool loadImageTransform();
    bool saveImageTransform();

    bool loadBasicParam();
    bool saveBasicParam();
};

#endif /* SRC_DATACENTER_H_ */
