#include <iostream>

#include <opencv2/opencv.hpp>

#include "DataCenter.h"
#include "ColorSegmentation.h"

#define IMAGE_WIDTH     512
#define IMAGE_HEIGHT    384

const std::string aboutString = "LocalizationServer v0.1.0";
const std::string paramKeys =
    "{help h    |   |Print help}"
    "{video     |   |Video index}";

void ValueCallback(int thres, void* in_data)
{
    double* value = (double*)in_data;
    *value = double(thres);
}

void onMouse(int event, int x, int y, int flags, void* in_data)
{
    if(y >= 10 && y <= IMAGE_HEIGHT+10 && x >= 10 && x <= IMAGE_WIDTH+10)
    {
        cv::Mat yuvImage;
        cv::Mat *pImage = (cv::Mat*)(in_data);

        int yValue, uValue, vValue;
        cv::cvtColor(*pImage, yuvImage, cv::COLOR_BGR2YUV);
        yValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[0];
        uValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[1];
        vValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[2];

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
    DataCenter dataCenter;
    //dataCenter.loadParam();
    ColorSegmentation colorSegmentation;

    cv::Mat rawImage, smallImage;
    cv::Size imageSize(IMAGE_WIDTH, IMAGE_HEIGHT);

    int button;
    std::vector<cv::Point2d> points;

    cv::Scalar minValue, maxValue;
    cv::Scalar *pMinValue, *pMaxValue;

    std::string text;

    bool running;

    cv::CommandLineParser parser(argc, argv, paramKeys);
    helpMessage(parser);

    uint8_t videoIndex = parser.get<uint8_t>("video");
    cv::VideoCapture camera(videoIndex);
    camera >> rawImage;

    cv::namedWindow("BigImage");
    cv::setMouseCallback("BigImage", onMouse, (void*)(&smallImage));

    cv::Mat bigImage(imageSize.height*2+30, imageSize.width*2+30, rawImage.type());
    cv::Rect topLeftRect(cv::Point(10, 10), imageSize);
    cv::Rect topRightRect(cv::Point(10+imageSize.width+10, 10), imageSize);
    cv::Rect bottomLeftRect(cv::Point(10, 10+imageSize.height+10), imageSize);
    cv::Rect bottomRightRect(cv::Point(10+imageSize.width+10, 10+imageSize.height+10), imageSize);

    cv::namedWindow("Config", cv::WINDOW_AUTOSIZE);
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

    running = true;
    while(running)
    {
        camera >> rawImage;
        cv::resize(rawImage, smallImage, imageSize);
        smallImage.copyTo(bigImage(topLeftRect));
        smallImage.copyTo(bigImage(bottomLeftRect));

        pMinValue = &minValue;
        pMaxValue = &maxValue;
        colorSegmentation.setThreshold(*pMinValue, *pMaxValue);
        colorSegmentation.getBlocks(bigImage(topLeftRect), points);
        colorSegmentation.drawPoints(bigImage(bottomLeftRect), cv::Scalar(0, 255, 255));
        colorSegmentation.drawContours(bigImage(bottomLeftRect), cv::Scalar(255, 255 ,0));
        cv::cvtColor(colorSegmentation.m_thresholdImage, bigImage(topRightRect), cv::COLOR_GRAY2BGR);
        cv::cvtColor(colorSegmentation.m_filterImage, bigImage(bottomRightRect), cv::COLOR_GRAY2BGR);

        cv::imshow("BigImage", bigImage);
        button = cv::waitKey(10);
        switch(button)
        {
            case 49:
                text = "Team A";
                pMinValue = &dataCenter.m_teamAMin;
                pMaxValue = &dataCenter.m_teamAMax;
                break;
            case 50:
                text = "Team B";
                pMinValue = &dataCenter.m_teamBMin;
                pMaxValue = &dataCenter.m_teamBMax;
                break;
            case 51:
                text = "Number One";
                pMinValue = &dataCenter.m_teamNumb1Min;
                pMaxValue = &dataCenter.m_teamNumb1Max;
                break;
            case 52:
                text = "Number Two";
                pMinValue = &dataCenter.m_teamNumb1Min;
                pMaxValue = &dataCenter.m_teamNumb2Max;
                break;
            case 113:
            case 10:
                running = false;
        }
    }

    dataCenter.saveParam();

    return 0;
}
