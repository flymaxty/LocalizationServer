/*
 * Broadcaster.cpp
 *
 *  Created on: Feb 2, 2016
 *      Author: ye
 */

#include <iostream>
#include "string.h"

#include "cJSON/cJSON.h"

#include "Broadcaster.h"
#include "common.h"

Broadcaster::Broadcaster(const char *in_id, const char *in_topic,
        const char * in_host, int in_port): mosquittopp(in_id)
{
    m_topic = in_topic;
    m_host = in_host;
    m_port = in_port;

    mosqpp::lib_init();
}

Broadcaster::~Broadcaster()
{
    disconnect();
}

bool Broadcaster::connectServer()
{
    //connect(m_host, m_port);
    //std::cout << connect(m_host, m_port) << std::endl;
    connect(m_host, m_port);
    LOG(INFO) << "MQTT connect to " << m_host << ":" << m_port;
    return true;
}

void Broadcaster::publishLocalization(RobotTeam& in_teamA, RobotTeam& in_teamB, std::vector<Obstacle> in_obstacles)
{
    //cJSON *root, *elem;

    cJSON *root = cJSON_CreateArray(), *elem;
    for(int i=0; i<4; i++)
    {
        if(in_teamB.robots[i].online)
        {
            cJSON_AddItemToArray(root, elem = cJSON_CreateObject());
            cJSON_AddNumberToObject(elem, "type", 0);
            cJSON_AddNumberToObject(elem, "id", in_teamB.robots[i].id);
            cJSON_AddNumberToObject(elem, "x", in_teamB.robots[i].x);
            cJSON_AddNumberToObject(elem, "y", in_teamB.robots[i].y);
            cJSON_AddNumberToObject(elem, "theta", in_teamB.robots[i].theta);
        }
    }

    //cJSON_Delete(elem);
    //std::cout << "lalla" << std::endl;

    for(int i=0; i<4; i++)
    {
        if(in_teamA.robots[i].online)
        {
            cJSON_AddItemToArray(root, elem = cJSON_CreateObject());
            cJSON_AddNumberToObject(elem, "type", MOSQUITTO_ROBOT_TYPE);
            cJSON_AddNumberToObject(elem, "id", in_teamA.robots[i].id+4);
            cJSON_AddNumberToObject(elem, "x", in_teamA.robots[i].x);
            cJSON_AddNumberToObject(elem, "y", in_teamA.robots[i].y);
            cJSON_AddNumberToObject(elem, "theta", in_teamA.robots[i].theta);
        }
    }

    for(int i=0; i<in_obstacles.size(); i++)
    {
        cJSON_AddItemToArray(root, elem = cJSON_CreateObject());
        cJSON_AddNumberToObject(elem, "type", MOSQUITTO_OBSTACLE_TYPE);
        cJSON_AddNumberToObject(elem, "id", in_obstacles[i].id);
        cJSON_AddNumberToObject(elem, "x", in_obstacles[i].x);
        cJSON_AddNumberToObject(elem, "y", in_obstacles[i].y);
    }

    char* buffer = cJSON_Print(root);
    publish(NULL, m_topic, strlen(buffer), reinterpret_cast<const uint8_t*>(buffer), 1, false);

    //std::string bufferS(buffer);

    //std::cout << bufferS << std::endl;

    cJSON_Delete(root);
}
