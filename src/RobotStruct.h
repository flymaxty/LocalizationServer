/*
 * RobotStruct.h
 *
 *  Created on: Dec 24, 2015
 *      Author: ye
 */

#ifndef SRC_ROBOTSTRUCT_H_
#define SRC_ROBOTSTRUCT_H_

//Robot Struct
struct Robot
{
	uint8_t id;
	bool online;
	double x;
	double y;
	double theta;
};

//Team Struct
struct RobotTeam
{
	uint8_t onlineNumbuer;
	Robot robots[4];
};

#endif /* SRC_ROBOTSTRUCT_H_ */
