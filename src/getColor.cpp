#include <iostream>

#include <opencv2/opencv.hpp>

#include "ColorSegmentation.h"

const std::string keys =
    "{help h    |   |print help}"
    "{image     |   |source image}"
    ;

cv::Mat rawImage, tempImage, outputImage;

cv::FileStorage fs;

int colorNumb = 0;
std::string colorState = "Team Red";

ColorSegmentation colorSegmentation;

cv::Scalar minValue, maxValue;

void ValueCallback(int thres, void* in_data)
{
	double* value = (double*)in_data;
	*value = double(thres);
}

void onMouse(int event, int x, int y, int flags, void* in_param)
{
    if(y >= 10 && y <= 263 && x >= 10 && x <= 394)
    {
    	cv::Mat yuvImage;
    	int yValue, uValue, vValue;

    	//rawImage.copyTo(yuvImage);
    	cv::cvtColor(rawImage, yuvImage, cv::COLOR_BGR2YUV);
		yValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[0];
		uValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[1];
		vValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[2];

		std::ostringstream yuvTextStream;
		yuvTextStream << "Y: " << yValue << " U: " << uValue << " V: " << vValue;
		std::string yuvText = yuvTextStream.str();

		rawImage.copyTo(tempImage);
		cv::putText(tempImage, yuvText, cv::Point(0,230),
				cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 1.5, cv::LINE_AA);
		cv::putText(tempImage, colorState, cv::Point(0,248),
				cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 1.5, cv::LINE_AA);

		if(event == cv::EVENT_LBUTTONUP && colorNumb < 4)
		{
	    	std::cout << "min: " << minValue << std::endl;
	    	std::cout << "max: " << maxValue << std::endl;
			if(colorNumb == 0)
			{
				fs << "TeamRedMin" << minValue;
				fs << "TeamRedMax" << maxValue;
				colorState = "Team Green";
				colorNumb++;
			}
			else if(colorNumb == 1)
			{
				fs << "TeamGreenMin" << minValue;
				fs << "TeamGreenMax" << maxValue;
				colorState = "Number One";
				colorNumb++;
			}
			else if(colorNumb == 2)
			{
				fs << "TeamNumb1Min" << minValue;
				fs << "TeamNumb1Max" << maxValue;
				colorState = "Number Two";
				colorNumb++;
			}
			else if(colorNumb == 3)
			{
				fs << "TeamNumb2Min" << minValue;
				fs << "TeamNumb2Max" << maxValue;
				colorState = "All Done";
				fs.release();
				colorNumb++;
			}
		}
		else if(event == cv::EVENT_RBUTTONUP)
		{
			cv::setTrackbarPos("yMinValue", "Config", yValue - 10);
			cv::setTrackbarPos("uMinValue", "Config", uValue - 10);
			cv::setTrackbarPos("vMinValue", "Config", vValue - 10);
			cv::setTrackbarPos("yMaxValue", "Config", yValue + 10);
			cv::setTrackbarPos("uMaxValue", "Config", uValue + 10);
			cv::setTrackbarPos("vMaxValue", "Config", vValue + 10);
		}
	}
}

int main(int argc, char** argv)
{
	fs = cv::FileStorage("threshold.yaml", cv::FileStorage::WRITE);
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("LocalizationServer v0.1.0");

    if(parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    std::string imageName = parser.get<std::string>("image");
    std::cout << "Image Name: " << imageName << std::endl;
    rawImage = cv::imread(imageName);
    cv::resize(rawImage, rawImage, cv::Size(384, 253));
    rawImage.copyTo(tempImage);


    cv::namedWindow("BigImage");
    cv::setMouseCallback("BigImage", onMouse, NULL);

	cv::Mat bigImage(536, 798, rawImage.type());
	cv::Rect image1(10, 10, 384, 253);
	cv::Rect image2(404, 10, 384, 253);
	cv::Rect image3(10, 273, 384, 253);
	cv::Rect image4(404, 273, 384, 253);

	//cv::Scalar minValue, maxValue;

	cv::namedWindow("Config", cv::WINDOW_AUTOSIZE);
	cv::createTrackbar("yMinValue", "Config", NULL,
						255, ValueCallback, (void*)(&minValue.val[0]));
	cv::createTrackbar("uMinValue", "Config", NULL,
						255, ValueCallback, (void*)(&minValue.val[1]));
	cv::createTrackbar("vMinValue", "Config", NULL,
						255, ValueCallback, (void*)(&minValue.val[2]));
	cv::createTrackbar("yMaxValue", "Config", NULL,
						255, ValueCallback, (void*)(&maxValue.val[0]));
	cv::createTrackbar("uMaxValue", "Config", NULL,
						255, ValueCallback, (void*)(&maxValue.val[1]));
	cv::createTrackbar("vMaxValue", "Config", NULL,
						255, ValueCallback, (void*)(&maxValue.val[2]));

	std::vector<cv::Point2d> points;
    while(1)
    {
    	rawImage.copyTo(outputImage);
    	colorSegmentation.setThreshold(minValue, maxValue);

    	points.clear();
    	colorSegmentation.getBlocks(rawImage, points);
    	colorSegmentation.drawPoints(outputImage, cv::Scalar(0, 255, 255));
    	colorSegmentation.drawContours(outputImage, cv::Scalar(255, 255 ,0));

        tempImage.copyTo(bigImage(image1));
        cv::cvtColor(colorSegmentation.m_thresholdImage, bigImage(image2), cv::COLOR_GRAY2BGR);
        cv::cvtColor(colorSegmentation.m_filterImage, bigImage(image4), cv::COLOR_GRAY2BGR);
        outputImage.copyTo(bigImage(image3));
        cv::imshow("BigImage", bigImage);

        cv::waitKey(1);
    }

    return 0;
}
