#include <iostream>
#include <sys/time.h>

#include <opencv2/opencv.hpp>

#include "DataCenter.h"
#include "ColorSegmentation.h"
#include "TeamDetect.h"
#include "Image2World.h"

DataCenter dataCenter;
ColorSegmentation redSegmentation;
ColorSegmentation greenSegmentation;
ColorSegmentation numb1Segmentation;
ColorSegmentation numb2Segmentation;
TeamDetect teamDetect;

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

    dataCenter.loadAllParam();
    redSegmentation.setThreshold(dataCenter.m_teamAMin, dataCenter.m_teamAMax);
    greenSegmentation.setThreshold(dataCenter.m_teamBMin, dataCenter.m_teamBMax);
    numb1Segmentation.setThreshold(dataCenter.m_teamNumb1Min, dataCenter.m_teamNumb1Max);
    numb2Segmentation.setThreshold(dataCenter.m_teamNumb2Min, dataCenter.m_teamNumb2Max);

    int videoIndex = parser.get<int>("video");
    cv::VideoCapture camera(videoIndex);
    camera.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, 768);
    cv::Mat rawImage, undistorImage;
    int timeout = 20;
    while(timeout)
    {
        camera >> rawImage;
        timeout--;
    }

    std::vector<cv::Point2d> redPoints;
    std::vector<cv::Point2d> greenPoints;
    std::vector<cv::Point2d> numb1Points;
    std::vector<cv::Point2d> numb2Points;

    double timeUse;
    struct timeval startTime, stopTime;

    while(1)
    {
        std::cout << "================== Start ==================" << std::endl;
        gettimeofday(&startTime, NULL);

        camera >> rawImage;
        rawImage.copyTo(dataCenter.m_rawImage);

        //cv::undistort(rawImage, undistorImage, dataCenter.m_cameraMatrix, dataCenter.m_distCoeffs);

        redSegmentation.getBlocks(rawImage, redPoints);
        greenSegmentation.getBlocks(rawImage, greenPoints);
        numb1Segmentation.getBlocks(rawImage, numb1Points);
        numb2Segmentation.getBlocks(rawImage, numb2Points);

        /*redSegmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 0, 255));
        greenSegmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 255, 0));
        numb1Segmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(255, 255, 0));
        numb2Segmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 255, 255));*/

        teamDetect.getTeam(redPoints, numb1Points, numb2Points, dataCenter.m_teamA);
        teamDetect.drawTeam(dataCenter.m_rawImage, cv::Scalar(0, 0, 255), dataCenter.m_teamA);
        teamDetect.getTeam(greenPoints, numb1Points, numb2Points, dataCenter.m_teamB);
        teamDetect.drawTeam(dataCenter.m_rawImage, cv::Scalar(0, 255, 0), dataCenter.m_teamB);

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

        gettimeofday(&stopTime, NULL);
        timeUse = (stopTime.tv_sec - startTime.tv_sec)*1000000.0 + (stopTime.tv_usec - startTime.tv_usec);
        std::cout << "FPS: " << 1000000.0 / timeUse << std::endl;

        cv::imshow("Test", dataCenter.m_rawImage);
        //cv::imshow("undistort", undistorImage);
        cv::waitKey(1);
    }

    return 0;
}
