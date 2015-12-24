#include <iostream>

#include <opencv2/opencv.hpp>

#include "DataCenter.h"
#include "ColorSegmentation.h"

DataCenter dataCenter;
ColorSegmentation redSegmentation;
ColorSegmentation greenSegmentation;
ColorSegmentation numb1Segmentation;
ColorSegmentation numb2Segmentation;

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
        rawImage.copyTo(dataCenter.m_rawImage);
        redSegmentation.getBlocks(rawImage, redPoints);
        std::cout << "redPoints: " << redPoints.size() << std::endl;
        greenSegmentation.getBlocks(rawImage, greenPoints);
        numb1Segmentation.getBlocks(rawImage, numb1Points);
        numb2Segmentation.getBlocks(rawImage, numb2Points);
        redSegmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 0, 255));
        greenSegmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 255, 0));
        numb1Segmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(255, 255, 0));
        numb2Segmentation.drawPoints(dataCenter.m_rawImage, cv::Scalar(0, 255, 255));
        cv::imshow("Test", dataCenter.m_rawImage);
        cv::waitKey(1);
    }

    return 0;
}
