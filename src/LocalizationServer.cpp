#include <iostream>
#include <sys/time.h>

#include <opencv2/opencv.hpp>

#include "cJSON/cJSON.h"

#include "DataCenter.h"
#include "ColorSegmentation.h"
#include "TeamDetect.h"
#include "Image2World.h"

const std::string aboutString = "LocalizationServer v0.1.0";
const std::string paramKeys =
    "{help h    |   |Print help}"
    "{video     |   |Video index}";

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
    ColorSegmentation redSegmentation;
    ColorSegmentation greenSegmentation;
    ColorSegmentation numb1Segmentation;
    ColorSegmentation numb2Segmentation;
    TeamDetect teamDetect;

    dataCenter.loadAllParam();
    redSegmentation.setThreshold(dataCenter.m_teamAMin, dataCenter.m_teamAMax);
    greenSegmentation.setThreshold(dataCenter.m_teamBMin, dataCenter.m_teamBMax);
    numb1Segmentation.setThreshold(dataCenter.m_teamNumb1Min, dataCenter.m_teamNumb1Max);
    numb2Segmentation.setThreshold(dataCenter.m_teamNumb2Min, dataCenter.m_teamNumb2Max);

    Image2World image2World;
    image2World.m_cameraMatrix = dataCenter.m_cameraMatrix;
    image2World.m_distCoeffs = dataCenter.m_distCoeffs;
    image2World.m_transMat = dataCenter.m_transMatrix;
    image2World.m_fieldHeight = dataCenter.m_fieldHeight;
    image2World.m_fieldWidth = dataCenter.m_fieldWidth;
    image2World.m_imageHeight = dataCenter.m_imageHeight;
    image2World.m_imageWidth = dataCenter.m_imageWidth;

    int videoIndex = parser.get<int>("video");
    cv::VideoCapture camera(videoIndex);
    camera.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 768);
    for(int timeout=20; timeout > 0; timeout--)
    {
        camera.grab();
    }

    cv::namedWindow("final", cv::WINDOW_KEEPRATIO);
    cv::Mat rawImage, realImage, fieldImage;
    fieldImage = cv::Mat::zeros(cv::Size(1540, 1340), CV_64FC3);

    std::vector<cv::Point2d> redPoints, undistortRedPoints, realRedPoints;
    std::vector<cv::Point2d> greenPoints, undistortGreenPoints, realGreenPoints;
    std::vector<cv::Point2d> numb1Points, undistortNumb1Points, realNumb1Points;
    std::vector<cv::Point2d> numb2Points, undistortNumb2Points, realNumb2Points;

    double timeUse;
    struct timeval startTime, stopTime;

    cJSON *root, *elem;

    while(1)
    {
        std::cout << "================== Start ==================" << std::endl;
        gettimeofday(&startTime, NULL);

        camera >> rawImage;
        rawImage.copyTo(realImage);
        fieldImage.setTo(0);

        redSegmentation.getBlocks(rawImage, redPoints);
        greenSegmentation.getBlocks(rawImage, greenPoints);
        numb1Segmentation.getBlocks(rawImage, numb1Points);
        numb2Segmentation.getBlocks(rawImage, numb2Points);

        redSegmentation.drawPoints(realImage, cv::Scalar(0, 0, 255));
        greenSegmentation.drawPoints(realImage, cv::Scalar(0, 255, 0));
        numb1Segmentation.drawPoints(realImage, cv::Scalar(255, 255, 0));
        numb2Segmentation.drawPoints(realImage, cv::Scalar(0, 255, 255));

		image2World.convert2Field(redPoints, realRedPoints);
		image2World.convert2Field(greenPoints, realGreenPoints);
		image2World.convert2Field(numb1Points, realNumb1Points);
		image2World.convert2Field(numb2Points, realNumb2Points);

        teamDetect.getTeam(realRedPoints, realNumb1Points, realNumb2Points, dataCenter.m_teamA);
        //teamDetect.drawTeam(fieldImage, cv::Scalar(0, 0, 255), dataCenter.m_teamA);
        teamDetect.getTeam(realGreenPoints, realNumb1Points, realNumb2Points, dataCenter.m_teamB);
        //teamDetect.drawTeam(fieldImage, cv::Scalar(0, 255, 0), dataCenter.m_teamB);

        for(int i=0; i<undistortRedPoints.size(); i++)
        {
        	std::cout << "Raw: " << redPoints[i] << std::endl;
        	std::cout << "Rel: " << realRedPoints[i] << std::endl;
        }

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

        root = cJSON_CreateArray();
        for(int i=0; i<4; i++)
        {
            if(dataCenter.m_teamB.robots[i].online)
            {
            	cJSON_AddItemToArray(root, elem = cJSON_CreateObject());
        		cJSON_AddNumberToObject(elem, "id", dataCenter.m_teamB.robots[i].id);
        		cJSON_AddNumberToObject(elem, "x", dataCenter.m_teamB.robots[i].x);
        		cJSON_AddNumberToObject(elem, "y", dataCenter.m_teamB.robots[i].y);
        		cJSON_AddNumberToObject(elem, "theta", dataCenter.m_teamB.robots[i].theta);
            }
        }

        //cJSON_Delete(elem);
        //std::cout << "lalla" << std::endl;

        for(int i=0; i<4; i++)
        {
            if(dataCenter.m_teamA.robots[i].online)
            {
            	cJSON_AddItemToArray(root, elem = cJSON_CreateObject());
        		cJSON_AddNumberToObject(elem, "id", dataCenter.m_teamA.robots[i].id+4);
        		cJSON_AddNumberToObject(elem, "x", dataCenter.m_teamA.robots[i].x);
        		cJSON_AddNumberToObject(elem, "y", dataCenter.m_teamA.robots[i].y);
        		cJSON_AddNumberToObject(elem, "theta", dataCenter.m_teamA.robots[i].theta);
            }
        }

        char* buffer = cJSON_Print(root);
        std::string bufferS(buffer);

        std::cout << bufferS << std::endl;

        cJSON_Delete(root);

        gettimeofday(&stopTime, NULL);
        timeUse = (stopTime.tv_sec - startTime.tv_sec)*1000000.0 + (stopTime.tv_usec - startTime.tv_usec);
        std::cout << "FPS: " << 1000000.0 / timeUse << std::endl;

        //cv::imshow("Test", realImage);
        //cv::imshow("final", fieldImage);
        //cv::waitKey(1);
    }

    return 0;
}
