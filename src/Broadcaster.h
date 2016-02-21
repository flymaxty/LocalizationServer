/*
 * Broadcaster.h
 *
 *  Created on: Feb 2, 2016
 *      Author: ye
 */

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
