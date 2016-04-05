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

#ifndef SRC_ROBOTSTRUCT_H_
#define SRC_ROBOTSTRUCT_H_

//Mosquitto define
#define MOSQUITTO_ROBOT_TYPE 0
#define MOSQUITTO_OBSTACLE_TYPE 1

//Obstacle Struct
struct Obstacle
{
	uint8_t id;
	double x;
	double y;
};

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
