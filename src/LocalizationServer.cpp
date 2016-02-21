#include <iostream>
#include <string>
#include <sys/time.h>

#include <opencv2/opencv.hpp>

#include "cJSON/cJSON.h"

#include "RobotStruct.h"
#include "DataCenter.h"
#include "ColorSegmentation.h"
#include "TeamDetect.h"
#include "Image2World.h"
#include "Broadcaster.h"

const std::string aboutString = "LocalizationServer v0.1.0";
const std::string paramKeys =
    "{help h    |   |Print help}";

void helpMessage(cv::CommandLineParser& in_parser)
{
    if(in_parser.has("help"))
    {
        in_parser.about(aboutString);
        in_parser.printMessage();
        exit(0);
    }
}

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, paramKeys);
    helpMessage(parser);

    DataCenter dataCenter;
    ColorSegmentation obsSegmentation;
    ColorSegmentation redSegmentation;
    ColorSegmentation greenSegmentation;
    ColorSegmentation numb1Segmentation;
    ColorSegmentation numb2Segmentation;

    obsSegmentation.setThreshold(dataCenter.m_cartesianMin, dataCenter.m_cartesianMax);
    redSegmentation.setThreshold(dataCenter.m_teamAMin, dataCenter.m_teamAMax);
    greenSegmentation.setThreshold(dataCenter.m_teamBMin, dataCenter.m_teamBMax);
    numb1Segmentation.setThreshold(dataCenter.m_teamNumb1Min, dataCenter.m_teamNumb1Max);
    numb2Segmentation.setThreshold(dataCenter.m_teamNumb2Min, dataCenter.m_teamNumb2Max);

    Image2World image2World(&dataCenter);

    TeamDetect teamDetect;

    cv::VideoCapture camera;
    if(dataCenter.m_cameraString.length() == 1)
    {
    	int videoIndex = std::stoi(dataCenter.m_cameraString);
    	camera.open(videoIndex);
    }
    else
    {
    	camera.open(dataCenter.m_cameraString);
    }
    camera.set(cv::CAP_PROP_FRAME_WIDTH, dataCenter.m_imageWidth);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, dataCenter.m_imageHeight);
    for(int timeout=60; timeout > 0; timeout--)
    {
        camera.grab();
    }

    //cv::namedWindow("LocalizationServer", cv::WINDOW_KEEPRATIO);
    cv::Mat rawImage, realImage;

    std::vector<cv::Point2d> obsPoints, realObsPoints;
    std::vector<cv::Point2d> redPoints, realRedPoints;
    std::vector<cv::Point2d> greenPoints, realGreenPoints;
    std::vector<cv::Point2d> numb1Points, realNumb1Points;
    std::vector<cv::Point2d> numb2Points, realNumb2Points;

    double timeUse;
    struct timeval startTime, stopTime;

	Broadcaster bk(dataCenter.m_mqttNodeName.c_str(),
			dataCenter.m_mqttLocalizationTopic.c_str(),
			dataCenter.m_mqttMasterIP.c_str());
	bk.connectServer();

    int button;
    bool running = true;
    while(running)
    {
        std::cout << "================== Start ==================" << std::endl;
        gettimeofday(&startTime, NULL);

        camera >> rawImage;
        rawImage.copyTo(realImage);

        obsSegmentation.getBlocks(rawImage, obsPoints);
        redSegmentation.getBlocks(rawImage, redPoints);
        greenSegmentation.getBlocks(rawImage, greenPoints);
        numb1Segmentation.getBlocks(rawImage, numb1Points);
        numb2Segmentation.getBlocks(rawImage, numb2Points);

        obsSegmentation.drawPoints(realImage, cv::Scalar(255, 255, 255));
        redSegmentation.drawPoints(realImage, cv::Scalar(0, 0, 255));
        greenSegmentation.drawPoints(realImage, cv::Scalar(0, 255, 0));
        numb1Segmentation.drawPoints(realImage, cv::Scalar(255, 255, 0));
        numb2Segmentation.drawPoints(realImage, cv::Scalar(0, 255, 255));

        obsSegmentation.drawContours(realImage, cv::Scalar(255, 255, 255));
        redSegmentation.drawContours(realImage, cv::Scalar(0, 0, 255));
        greenSegmentation.drawContours(realImage, cv::Scalar(0, 255, 0));
        numb1Segmentation.drawContours(realImage, cv::Scalar(255, 255, 0));
        numb2Segmentation.drawContours(realImage, cv::Scalar(0, 255, 255));

        image2World.convert2Field(obsPoints, realObsPoints, dataCenter.m_obsHeight);
		image2World.convert2Field(redPoints, realRedPoints, dataCenter.m_robotHeight);
		image2World.convert2Field(greenPoints, realGreenPoints, dataCenter.m_robotHeight);
		image2World.convert2Field(numb1Points, realNumb1Points, dataCenter.m_robotHeight);
		image2World.convert2Field(numb2Points, realNumb2Points, dataCenter.m_robotHeight);


		dataCenter.m_obstacles.clear();
		Obstacle tmpObstacle;
		for(int j=0; j<realObsPoints.size(); j++)
		{
			if(realObsPoints[j].x > 0 && realObsPoints[j].x < dataCenter.m_imageWidth
					&& realObsPoints[j].y > 0 && realObsPoints[j].y < dataCenter.m_imageHeight)
			{
				tmpObstacle.id = dataCenter.m_obstacles.size();
				tmpObstacle.x = realObsPoints[j].x;
				tmpObstacle.y = realObsPoints[j].y;
				dataCenter.m_obstacles.push_back(tmpObstacle);

				std::cout << realObsPoints[j] << std::endl;
			}
		}
/*		std::cout << realRedPoints << std::endl;
		std::cout << realGreenPoints << std::endl;
		std::cout << realNumb1Points << std::endl;
		std::cout << realNumb2Points << std::endl;
*/
		teamDetect.getTeam(realRedPoints, realNumb1Points, realNumb2Points, dataCenter.m_teamA);
		//teamDetect.drawTeam(fieldImage, cv::Scalar(0, 0, 255), dataCenter.m_teamA);

		teamDetect.getTeam(realGreenPoints, realNumb1Points, realNumb2Points, dataCenter.m_teamB);
		//teamDetect.drawTeam(fieldImage, cv::Scalar(0, 255, 0), dataCenter.m_teamB);

        std::cout << "> Red Team <" << std::endl;
        for(int i=0; i<4; i++)
        {
            if(dataCenter.m_teamA.robots[i].online)
            {
                std::cout << "id: " << int(dataCenter.m_teamA.robots[i].id);
                std::cout << ",\t x: " << dataCenter.m_teamA.robots[i].x;
                std::cout << ",\t y: " << dataCenter.m_teamA.robots[i].y;
                std::cout << ",\t th: " << dataCenter.m_teamA.robots[i].theta;
                std::cout << std::endl;
            }
        }
        std::cout << "> Green Team <" << std::endl;
        for(int i=0; i<4; i++)
        {
            if(dataCenter.m_teamB.robots[i].online)
            {
                std::cout << "id: " << int(dataCenter.m_teamB.robots[i].id);
                std::cout << ",\t x: " << dataCenter.m_teamB.robots[i].x;
                std::cout << ",\t y: " << dataCenter.m_teamB.robots[i].y;
                std::cout << ",\t th: " << dataCenter.m_teamB.robots[i].theta;
                std::cout << std::endl;
            }
        }
        std::cout << "> Obstacle <" << std::endl;
        for(int i=0; i<dataCenter.m_obstacles.size(); i++)
        {
			std::cout << "id: " << int(dataCenter.m_obstacles[i].id);
			std::cout << ",\t x: " << dataCenter.m_obstacles[i].x;
			std::cout << ",\t y: " << dataCenter.m_obstacles[i].y;
			std::cout << std::endl;
        }

        gettimeofday(&stopTime, NULL);
        timeUse = (stopTime.tv_sec - startTime.tv_sec)*1000000.0 + (stopTime.tv_usec - startTime.tv_usec);
        std::cout << "FPS: " << 1000000.0 / timeUse << std::endl;

    	bk.publishLocalization(dataCenter.m_teamA, dataCenter.m_teamB, dataCenter.m_obstacles);

        cv::imshow("LocalizationServer", realImage);
        button = cv::waitKey(1);
    	if(button == 113 || button == 10)
    	{
    		running = false;
    	}
    }

    camera.release();

    return 0;
}
