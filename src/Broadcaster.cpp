/*
 * Broadcaster.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: ye
 */

#include "Broadcaster.h"

Broadcaster::Broadcaster(const char *in_id, const char *in_topic,
		const char * in_host, int in_port): mosquittopp(in_id)
{
	topic = in_topic;
	host = in_host;
	port = in_port;
}

Broadcaster::~Broadcaster()
{

}
