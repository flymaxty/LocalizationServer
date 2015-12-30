/*
 * TeamDetect.h
 *
 *  Created on: Dec 24, 2015
 *      Author: ye
 */

#ifndef SRC_TEAMDETECT_H_
#define SRC_TEAMDETECT_H_

#include <opencv2/opencv.hpp>

#include "RobotStruct.h"

class TeamDetect {
private:
    struct PointWithDis
    {
        cv::Point2d point;
        double distance;
    };
public:

public:
    TeamDetect();
    ~TeamDetect();

    bool initTeamState(RobotTeam& in_team);
    uint8_t getRobotId(uint16_t in_index1, uint16_t in_index2, uint16_t in_numb1End);
    bool getTeam(std::vector<cv::Point2d>& in_teamPoints, std::vector<cv::Point2d>& in_numb1Points,
            std::vector<cv::Point2d>& in_numb2Points, RobotTeam& in_team);
    uint16_t getNearestPoint(cv::Point2d in_point, std::vector<cv::Point2d> in_points, int16_t in_maskId=-1);
    Robot getTeamMember(cv::Point2d in_teamPoint, std::vector<cv::Point2d>& in_numbPoints,
            uint16_t in_numb1End);
    bool drawTeam(cv::Mat& in_image, cv::Scalar in_scalar, RobotTeam& in_team);
};

#endif /* SRC_TEAMDETECT_H_ */
