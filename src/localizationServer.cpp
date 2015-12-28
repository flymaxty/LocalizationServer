#include <iostream>

#include <opencv2/opencv.hpp>

#include "DataCenter.h"
#include "ColorSegmentation.h"
#include "TeamDetect.h"

DataCenter dataCenter;
ColorSegmentation redSegmentation;
ColorSegmentation greenSegmentation;
ColorSegmentation numb1Segmentation;
ColorSegmentation numb2Segmentation;
TeamDetect teamDetect;

const std::string keys =
    "{help h    |   |print help}"
    "{image     |   |source image}"
    ;

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("LocalizationServer v0.1.0");

    if(parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    dataCenter.loadParam();
    redSegmentation.setThreshold(dataCenter.m_teamRedMin, dataCenter.m_teamRedMax);
    greenSegmentation.setThreshold(dataCenter.m_teamGreenMin, dataCenter.m_teamGreenMax);
    numb1Segmentation.setThreshold(dataCenter.m_teamNumb1Min, dataCenter.m_teamNumb1Max);
    numb2Segmentation.setThreshold(dataCenter.m_teamNumb2Min, dataCenter.m_teamNumb2Max);

    //cv::VideoCapture camera(0);
    std::string imageName = parser.get<std::string>("image");
    cv::Mat rawImage = cv::imread(imageName);
    cv::Mat outputImage;

    std::vector<cv::Point2d> redPoints;
    std::vector<cv::Point2d> greenPoints;
    std::vector<cv::Point2d> numb1Points;
    std::vector<cv::Point2d> numb2Points;

    while(1)
    {
    	std::cout << "================== Start ==================" << std::endl;
        rawImage.copyTo(dataCenter.m_rawImage);

        redSegmentation.getBlocks(rawImage, redPoints);
        greenSegmentation.getBlocks(rawImage, greenPoints);
        numb1Segmentation.getBlocks(rawImage, numb1Points);
        numb2Segmentation.getBlocks(rawImage, numb2Points);

        //redSegmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 0, 255));
        //greenSegmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 255, 0));
        //numb1Segmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(255, 255, 0));
        //numb2Segmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 255, 255));

        teamDetect.getTeam(redPoints, numb1Points, numb2Points, dataCenter.m_teamRed);
        teamDetect.drawTeam(dataCenter.m_rawImage, cv::Scalar(0, 0, 255), dataCenter.m_teamRed);
        teamDetect.getTeam(greenPoints, numb1Points, numb2Points, dataCenter.m_teamGreen);
        teamDetect.drawTeam(dataCenter.m_rawImage, cv::Scalar(0, 255, 0), dataCenter.m_teamGreen);

        std::cout << "> Red Team <" << std::endl;
        for(int i=0; i<4; i++)
        {
        	if(dataCenter.m_teamRed.robots[i].online)
        	{
            	std::cout << "id: " << int(dataCenter.m_teamRed.robots[i].id);
            	std::cout << ",\t x: " << dataCenter.m_teamRed.robots[i].x;
            	std::cout << ",\t y: " << dataCenter.m_teamRed.robots[i].y;
            	std::cout << ",\t th: " << dataCenter.m_teamRed.robots[i].theta;
            	std::cout << std::endl;
        	}
        }
        std::cout << "> Green Team <" << std::endl;
        for(int i=0; i<4; i++)
        {
        	if(dataCenter.m_teamGreen.robots[i].online)
        	{
				std::cout << "id: " << int(dataCenter.m_teamGreen.robots[i].id);
				std::cout << ",\t x: " << dataCenter.m_teamGreen.robots[i].x;
				std::cout << ",\t y: " << dataCenter.m_teamGreen.robots[i].y;
				std::cout << ",\t th: " << dataCenter.m_teamGreen.robots[i].theta;
				std::cout << std::endl;
        	}
        }

        cv::imshow("Test", dataCenter.m_rawImage);
        cv::waitKey(1);
    }

    return 0;
}
