#include <iostream>
#include <sstream>
#include <unistd.h>

#include <opencv2/opencv.hpp>

#include "DataCenter.h"
#include "Image2World.h"
#include "ColorSegmentation.h"

const std::string aboutString = "GetColor v0.1.0";
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

void onMouse(int event, int x, int y, int, void* in_data)
{
	cv::Point2d *tmpPoint = static_cast<cv::Point2d*>(in_data);
	tmpPoint->x = x;
	tmpPoint->y = y;
}

void getNearest(cv::Point2d in_pointA, std::vector<cv::Point2d>& in_points, cv::Point2d* in_pointB)
{
	double tmp, distance = 9999;
	int number;
	for(int i=0; i<in_points.size(); i++)
	{
		tmp = sqrt((in_pointA.x-in_points[i].x)*(in_pointA.x-in_points[i].x) +
				(in_pointA.y-in_points[i].y)*(in_pointA.y-in_points[i].y) );
		if(tmp < distance)
		{
			number = i;
			distance = tmp;
		}
	}

	*in_pointB = in_points[number];
}

int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, paramKeys);
    helpMessage(parser);

    DataCenter dataCenter;

    ColorSegmentation colorSegmentation(dataCenter.m_cartesianMin, dataCenter.m_cartesianMax,
    		dataCenter.m_obsMinArea);

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

    cv::Mat rawImage, undistorImage;
    std::vector<cv::Point2d> points;
    cv::namedWindow("GetField", cv::WINDOW_KEEPRATIO);

    cv::Point2d tmpPoint;
    cv::setMouseCallback("GetField", onMouse, static_cast<void*>(&tmpPoint));

    int button;
    bool running = true;
    std::vector<cv::Point2d> vertex(4);
    while(running)
    {
    	camera >> rawImage;
    	cv::undistort(rawImage, undistorImage, dataCenter.m_cameraMatrix, dataCenter.m_distCoeffs);
    	colorSegmentation.getBlocks(undistorImage, points);
    	colorSegmentation.drawPoints(undistorImage, cv::Scalar(0,255,255));
    	button = cv::waitKey(1);
    	switch(button)
    	{
    	case 49:
    		getNearest(tmpPoint, points, &vertex[0]);
    		break;
    	case 50:
    		getNearest(tmpPoint, points, &vertex[1]);
			break;
    	case 51:
    		getNearest(tmpPoint, points, &vertex[2]);
			break;
    	case 52:
    		getNearest(tmpPoint, points, &vertex[3]);
			break;
    	case 113:
    	case 10:
    	    dataCenter.m_mapVertex = vertex;
    		running = false;
    	}

    	cv::line(undistorImage, vertex[0], vertex[1], cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
    	cv::line(undistorImage, vertex[1], vertex[2], cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
    	cv::line(undistorImage, vertex[2], vertex[3], cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
    	cv::line(undistorImage, vertex[3], vertex[0], cv::Scalar(0, 255, 255), 1, cv::LINE_AA);
    	for(int i=0; i<4; i++)
    	{
    		std::stringstream ss;
    		ss << i+1;
    		std::string text = ss.str();
    		cv::circle(undistorImage, vertex[i], 6, cv::Scalar(255, 0, 0), 2, cv::LINE_AA);
            cv::putText(undistorImage, text, vertex[i], cv::FONT_HERSHEY_DUPLEX, 1,
                    cv::Scalar(0, 0, 255), 1, cv::LINE_AA);
    	}

    	cv::imshow("GetField", undistorImage);
    }

    std::vector<cv::Point2d> mapVertex;
    mapVertex.push_back(cv::Point2d(0, 0));
    mapVertex.push_back(cv::Point2d(dataCenter.m_fieldWidth, 0));
    mapVertex.push_back(cv::Point2d(dataCenter.m_fieldWidth, dataCenter.m_fieldHeight));
    mapVertex.push_back(cv::Point2d(0, dataCenter.m_fieldHeight));

    Image2World image2World(&dataCenter);
    image2World.getTransMat(vertex, mapVertex, dataCenter.m_transMatrix);
    image2World.m_cameraMatrix = dataCenter.m_cameraMatrix;
    image2World.m_distCoeffs = dataCenter.m_distCoeffs;
    image2World.m_transMat = dataCenter.m_transMatrix;
    image2World.m_fieldHeight = dataCenter.m_fieldHeight;
    image2World.m_fieldWidth = dataCenter.m_fieldWidth;
    image2World.m_imageHeight = dataCenter.m_imageHeight;
    image2World.m_imageWidth = dataCenter.m_imageWidth;
    std::vector<cv::Point2d> imageCenter, imageCenterInMap;
    imageCenter.push_back(cv::Point2d(dataCenter.m_imageWidth/2, dataCenter.m_imageHeight/2));
    image2World.convert2Field(imageCenter, imageCenterInMap, 0.005);
    std::cout << imageCenterInMap << std::endl;
    dataCenter.m_imageCenterInMap = imageCenterInMap[0];
    dataCenter.saveImageTransform();

    std::vector<cv::Point2d> showVertex;
    showVertex.push_back(cv::Point2d(0, vertex[0].y - vertex[3].y));
    showVertex.push_back(cv::Point2d(vertex[1].x - vertex[0].x, vertex[0].y - vertex[3].y));
    showVertex.push_back(cv::Point2d(vertex[1].x - vertex[0].x, 0));
    showVertex.push_back(cv::Point2d(0, 0));

    cv::Mat showTransMatrix;
    image2World.getTransMat(vertex, showVertex, showTransMatrix);

    std::cout << vertex << std::endl;
    std::cout << mapVertex << std::endl;
    std::cout << showVertex << std::endl;
    std::cout << dataCenter.m_transMatrix << std::endl;
    std::cout << showTransMatrix << std::endl;

    cv::Mat finalImage;
    cv::namedWindow("Final", cv::WINDOW_KEEPRATIO);
    running = true;
    while(running)
    {
    	camera >> rawImage;
    	cv::undistort(rawImage, undistorImage, dataCenter.m_cameraMatrix, dataCenter.m_distCoeffs);
    	cv::warpPerspective(undistorImage, finalImage, showTransMatrix, cv::Size(showVertex[1].x, showVertex[1].y));

    	cv::imshow("Final", finalImage);
    	button = cv::waitKey(1);
    	if(button == 113 || button == 10)
    	{
    		running = false;
    	}
    }

    camera.release();

    return 0;
}
