/**
* The MIT License (MIT)
* 
* Copyright (c) 2016 Tian Ye
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
**/

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
    int8_t getRobotId(int16_t in_index1, int16_t in_index2, int16_t in_numb1End);
    bool getTeam(std::vector<cv::Point2d>& in_teamPoints, std::vector<cv::Point2d>& in_numb1Points,
            std::vector<cv::Point2d>& in_numb2Points, RobotTeam& in_team);
    int16_t getNearestPoint(cv::Point2d in_point, std::vector<cv::Point2d> in_points, int16_t in_maskId=-1);
    Robot getTeamMember(cv::Point2d in_teamPoint, std::vector<cv::Point2d>& in_numbPoints,
            uint16_t in_numb1End);
    bool drawTeam(cv::InputOutputArray in_image, cv::Scalar in_scalar, RobotTeam& in_team);
};

#endif /* SRC_TEAMDETECT_H_ */
