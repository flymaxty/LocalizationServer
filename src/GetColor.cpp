#include <iostream>

#include <opencv2/opencv.hpp>

#include "DataCenter.h"
#include "ColorSegmentation.h"

#define IMAGE_WIDTH     512
#define IMAGE_HEIGHT    384

const std::string aboutString = "GetColor v0.1.0";
const std::string paramKeys =
    "{help h    |   |Print help}";

double widthTimes;
double heightTimes;

void ValueCallback(int thres, void* in_data)
{
    double* value = (double*)in_data;
    *value = double(thres);
}

void updateTrackbarPos(cv::Scalar minValue, cv::Scalar maxValue)
{
    cv::setTrackbarPos("yMinValue", "Config", minValue[0]);
    cv::setTrackbarPos("yMaxValue", "Config", maxValue[0]);
    cv::setTrackbarPos("uMinValue", "Config", minValue[1]);
    cv::setTrackbarPos("uMaxValue", "Config", maxValue[1]);
    cv::setTrackbarPos("vMinValue", "Config", minValue[2]);
    cv::setTrackbarPos("vMaxValue", "Config", maxValue[2]);
}

void onMouse(int event, int x, int y, int flags, void* in_data)
{
    if(y >= 10 && y <= IMAGE_HEIGHT+10 && x >= 10 && x <= IMAGE_WIDTH+10)
    {
        cv::Mat yuvImage;
        cv::Mat *pImage = (cv::Mat*)(in_data);

        int yValue, uValue, vValue;
        cv::cvtColor(*pImage, yuvImage, cv::COLOR_BGR2YUV);
        double targetX = (x-10) * widthTimes;
        double targetY = (y-10) * heightTimes;
        yValue = yuvImage.at<cv::Vec3b>(targetY, targetX)[0];
        uValue = yuvImage.at<cv::Vec3b>(targetY, targetX)[1];
        vValue = yuvImage.at<cv::Vec3b>(targetY, targetX)[2];

        if(event == cv::EVENT_LBUTTONUP)
        {
            cv::setTrackbarPos("yMinValue", "Config", yValue - 10);
            cv::setTrackbarPos("yMaxValue", "Config", yValue + 10);
            cv::setTrackbarPos("uMinValue", "Config", uValue - 10);
            cv::setTrackbarPos("uMaxValue", "Config", uValue + 10);
            cv::setTrackbarPos("vMinValue", "Config", vValue - 10);
            cv::setTrackbarPos("vMaxValue", "Config", vValue + 10);
        }
    }
}

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
    widthTimes = dataCenter.m_imageWidth / IMAGE_WIDTH;
    heightTimes = dataCenter.m_imageHeight / IMAGE_HEIGHT;
    std::cout << widthTimes << std::endl;

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
    for(int timeout=20; timeout > 0; timeout--)
    {
        camera.grab();
    }
    cv::Mat rawImage;
    camera >> rawImage;

    cv::Mat bottomLeftImage, bottomRightImage, topRightImage;
    cv::namedWindow("BigImage", cv::WINDOW_KEEPRATIO);
    cv::setMouseCallback("BigImage", onMouse, (void*)(&rawImage));

    cv::Scalar minValue, maxValue;
    cv::Size imageSize(IMAGE_WIDTH, IMAGE_HEIGHT);
    cv::Mat bigImage(imageSize.height*2+30, imageSize.width*2+30, rawImage.type());
    cv::Rect topLeftRect(cv::Point(10, 10), imageSize);
    cv::Rect topRightRect(cv::Point(10+imageSize.width+10, 10), imageSize);
    cv::Rect bottomLeftRect(cv::Point(10, 10+imageSize.height+10), imageSize);
    cv::Rect bottomRightRect(cv::Point(10+imageSize.width+10, 10+imageSize.height+10), imageSize);

    cv::namedWindow("Config", cv::WINDOW_KEEPRATIO);
    cv::createTrackbar("yMinValue", "Config", NULL,
                        255, ValueCallback, (void*)(&minValue.val[0]));
    cv::createTrackbar("yMaxValue", "Config", NULL,
                        255, ValueCallback, (void*)(&maxValue.val[0]));
    cv::createTrackbar("uMinValue", "Config", NULL,
                        255, ValueCallback, (void*)(&minValue.val[1]));
    cv::createTrackbar("uMaxValue", "Config", NULL,
                        255, ValueCallback, (void*)(&maxValue.val[1]));
    cv::createTrackbar("vMinValue", "Config", NULL,
                        255, ValueCallback, (void*)(&minValue.val[2]));
    cv::createTrackbar("vMaxValue", "Config", NULL,
                        255, ValueCallback, (void*)(&maxValue.val[2]));

    std::string text = "Team A";
    cv::Scalar *pMinValue = &dataCenter.m_teamAMin;
    cv::Scalar *pMaxValue = &dataCenter.m_teamAMax;
    updateTrackbarPos(*pMinValue, *pMaxValue);


    ColorSegmentation colorSegmentation(*pMinValue, *pMaxValue, dataCenter.m_colorMinArea);

    int button;
    bool running = true;
    std::vector<cv::Point2d> points;
    while(running)
    {
        camera >> rawImage;
        rawImage.copyTo(bottomLeftImage);

        *pMinValue = minValue;
        *pMaxValue = maxValue;
        colorSegmentation.setThreshold(*pMinValue, *pMaxValue);
        colorSegmentation.getBlocks(rawImage, points);
        colorSegmentation.drawPoints(bottomLeftImage, cv::Scalar(0, 255, 255));
        colorSegmentation.drawContours(bottomLeftImage, cv::Scalar(255, 255 ,0));
        cv::cvtColor(colorSegmentation.m_thresholdImage, topRightImage, cv::COLOR_GRAY2BGR);
        cv::cvtColor(colorSegmentation.m_filterImage, bottomRightImage, cv::COLOR_GRAY2BGR);

        cv::resize(rawImage, bigImage(topLeftRect), imageSize);
        cv::resize(topRightImage, bigImage(topRightRect), imageSize);
        cv::resize(bottomLeftImage, bigImage(bottomLeftRect), imageSize);
        cv::resize(bottomRightImage, bigImage(bottomRightRect), imageSize);

        cv::putText(bigImage(topLeftRect), text, cv::Point(10, IMAGE_HEIGHT-10), cv::FONT_HERSHEY_DUPLEX, 1,
                cv::Scalar(0, 0, 255), 1, cv::LINE_AA);

        cv::imshow("BigImage", bigImage);
        button = cv::waitKey(10);
        switch(button)
        {
            case 49:
                text = "Team A";
                pMinValue = &dataCenter.m_teamAMin;
                pMaxValue = &dataCenter.m_teamAMax;
                updateTrackbarPos(*pMinValue, *pMaxValue);
                break;
            case 50:
                text = "Team B";
                pMinValue = &dataCenter.m_teamBMin;
                pMaxValue = &dataCenter.m_teamBMax;
                updateTrackbarPos(*pMinValue, *pMaxValue);
                break;
            case 51:
                text = "Number One";
                pMinValue = &dataCenter.m_teamNumb1Min;
                pMaxValue = &dataCenter.m_teamNumb1Max;
                updateTrackbarPos(*pMinValue, *pMaxValue);
                break;
            case 52:
                text = "Number Two";
                pMinValue = &dataCenter.m_teamNumb2Min;
                pMaxValue = &dataCenter.m_teamNumb2Max;
                updateTrackbarPos(*pMinValue, *pMaxValue);
                break;
            case 53:
                text = "Cartesian";
                pMinValue = &dataCenter.m_cartesianMin;
                pMaxValue = &dataCenter.m_cartesianMax;
                updateTrackbarPos(*pMinValue, *pMaxValue);
            break;
            case 113:
            case 10:
                running = false;
        }
    }

    dataCenter.saveSegmentationThreshold();
    camera.release();

    return 0;
}
