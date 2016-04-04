/*
 * Broadcaster_test.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: ye
 */

#include <iostream>

#include "common.h"

#include "Broadcaster.h"

int main(int argc, char** argv)
{
    Broadcaster bk("localization", "spider_viz", "127.0.0.1");
    bk.connectServer();
    const char* message="ha";
    bk.publish(NULL, "spider_viz", 2, reinterpret_cast<const uint8_t*>(message), 1, false);
    LOG(INFO) << "test logging";
    return 0;
}
