/*
 * Broadcaster.h
 *
 *  Created on: Feb 2, 2016
 *      Author: ye
 */

#ifndef BROADCASTER_H_
#define BROADCASTER_H_

#include <mosquittopp.h>

class Broadcaster: public mosquittopp::mosquittopp
{
public:
	const char* host;
	const char* topic;
	int port;
public:
	Broadcaster(const char *in_id, const char *in_topic, const char * in_host, int in_port=1883);
	~Broadcaster();
};

#endif /* BROADCASTER_H_ */
