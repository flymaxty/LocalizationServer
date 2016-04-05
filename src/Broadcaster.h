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

#ifndef BROADCASTER_H_
#define BROADCASTER_H_

#include <vector>

#include <mosquittopp.h>

#include "RobotStruct.h"

class Broadcaster: public mosquittopp::mosquittopp
{
public:
    const char* m_host;
    const char* m_topic;
    int m_port;
public:
    Broadcaster(const char *in_id, const char *in_topic, const char * in_host, int in_port=1883);
    virtual ~Broadcaster();
    bool connectServer();
    void publishLocalization(RobotTeam& in_teamA, RobotTeam& in_teamB, std::vector<Obstacle> in_obstacles);

};

#endif /* BROADCASTER_H_ */
