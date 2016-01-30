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
    cv::Mat m_cameraMatrix;
    cv::Mat m_distCoeffs;

    //Image
    cv::Mat m_rawImage;

    //Map
    cv::Mat m_transMatrix;
    std::vector<cv::Point2f> m_mapVertex;

public:
    std::string m_thresholdFileName;
    std::string m_cameraMatrixFileName;
    std::string m_matrixFileName;
public:
    DataCenter();
    ~DataCenter();

    bool loadAllParam();

    bool loadThreshold();
    bool saveThreshold();

    bool loadCameraMatrix();
    bool saveCameraMatrix();

    bool loadMatrix();
    bool saveMatrix();
};

#endif /* SRC_DATACENTER_H_ */
