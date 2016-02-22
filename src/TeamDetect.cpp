/*
 * TeamDetect.cpp
 *
 *  Created on: Dec 24, 2015
 *      Author: ye
 */

#include <sstream>
#include <math.h>

#include "TeamDetect.h"

TeamDetect::TeamDetect()
{

}

TeamDetect::~TeamDetect()
{

}

bool TeamDetect::initTeamState(RobotTeam& in_team)
{
    for(uint8_t i=0; i<4; i++)
    {
        in_team.onlineNumbuer = 0;
        in_team.robots[i].online = false;
    }

    return true;
}

int16_t TeamDetect::getNearestPoint(cv::Point2d in_point, std::vector<cv::Point2d> in_points, int16_t in_maskId)
{
    uint16_t diff = -1;
    cv::Point2d tempPoint;
    double tmpDistance=99999, distance;
    for(uint8_t i=0; i<in_points.size(); i++)
    {
        tempPoint = in_point - in_points[i];
        distance = cv::sqrt(tempPoint.x*tempPoint.x + tempPoint.y*tempPoint.y);
        if(distance < tmpDistance && i != in_maskId)
        {
            diff = i;
            tmpDistance = distance;
        }
    }

    return diff;
}

int8_t TeamDetect::getRobotId(int16_t in_index1, int16_t in_index2, int16_t in_numb1End)
{
    uint8_t id = -1;
    if(in_index1 <= in_numb1End)
    {
        if(in_index2 <= in_numb1End)
        {
            id = 0;
        }
        else
        {
            id = 1;
        }
    }
    else
    {
        if(in_index2 <= in_numb1End)
        {
            id = 2;
        }
        else
        {
            id = 3;
        }
    }

    return id;
}

Robot TeamDetect::getTeamMember(cv::Point2d in_teamPoint, std::vector<cv::Point2d>& in_numbPoints,
        uint16_t in_numb1End)
{
    Robot robot;
    int16_t index1, index2;

    index1 = getNearestPoint(in_teamPoint, in_numbPoints);
    index2 = getNearestPoint(in_teamPoint, in_numbPoints, index1);

    robot.id = getRobotId(index1, index2, in_numb1End);
    robot.online = true;
    robot.x = in_numbPoints[index1].x;
    robot.y = in_numbPoints[index1].y;
    robot.theta = atan2(in_teamPoint.y - robot.y, in_teamPoint.x-robot.x);
    return robot;
}

bool TeamDetect::getTeam(std::vector<cv::Point2d>& in_teamPoints, std::vector<cv::Point2d>& in_numb1Points,
            std::vector<cv::Point2d>& in_numb2Points, RobotTeam& in_team)
{
    Robot robot;
    initTeamState(in_team);
    std::vector<cv::Point2d> tempPoints;
    tempPoints.insert(tempPoints.begin(), in_numb1Points.begin(), in_numb1Points.end());
    tempPoints.insert(tempPoints.end(), in_numb2Points.begin(), in_numb2Points.end());
    for(uint8_t i=0; i<in_teamPoints.size(); i++)
    {
        robot = getTeamMember(in_teamPoints[i], tempPoints, in_numb1Points.size()-1);
        in_team.robots[robot.id] = robot;
        in_team.onlineNumbuer++;
    }
    return true;
}

bool TeamDetect::drawTeam(cv::InputOutputArray in_image, cv::Scalar in_scalar, RobotTeam& in_team)
{
	cv::Mat image = in_image.getMat();
    double radius = 20;
    int thickness = 1;

    for(uint8_t i=0; i<4; i++)
    {
        if(in_team.robots[i].online)
        {
            cv::circle(image, cv::Point2d(in_team.robots[i].x, in_team.robots[i].y),
                    radius, in_scalar, thickness, cv::LINE_AA);
            cv::line(image, cv::Point2d(in_team.robots[i].x, in_team.robots[i].y),
                    cv::Point2d(in_team.robots[i].x+cos(in_team.robots[i].theta)*radius, in_team.robots[i].y+sin(in_team.robots[i].theta)*radius),
                    in_scalar, thickness, cv::LINE_AA);
            std::stringstream ss;
            ss << int(in_team.robots[i].id);
            cv::putText(image, ss.str(), cv::Point2d(in_team.robots[i].x-30, in_team.robots[i].y-30),
                    cv::FONT_HERSHEY_DUPLEX, 0.8, in_scalar, 1, cv::LINE_AA);
        }
    }

    return true;
}
