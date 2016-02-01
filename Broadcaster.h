/*
 * Broadcaster.h
 *
 *  Created on: Feb 1, 2016
 *      Author: ye
 */

#ifndef BROADCASTER_H_
#define BROADCASTER_H_

#include <mosquittopp.h>

class Broadcaster: public mosquittopp::mosquittopp {
public:
	Broadcaster();
	~Broadcaster();
};

#endif /* BROADCASTER_H_ */
