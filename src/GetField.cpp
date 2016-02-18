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

    ColorSegmentation colorSegmentation;
    colorSegmentation.setThreshold(dataCenter.m_cartesianMin, dataCenter.m_cartesianMax);

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
    camera.set(cv::CAP_PROP_FRAME_WIDTH, dataCenter.m_fieldWidth);
    camera.set(cv::CAP_PROP_FRAME_HEIGHT, dataCenter.m_fieldHeight);
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
    //mapVertex.push_back(cv::Point2d(0, 0));
    //mapVertex.push_back(cv::Point2d(0, 1140));
    //mapVertex.push_back(cv::Point2d(1340, 1140));
    //mapVertex.push_back(cv::Point2d(1340, 0));

    mapVertex.push_back(cv::Point2d(0, 0.575));
    mapVertex.push_back(cv::Point2d(0, 0));
    mapVertex.push_back(cv::Point2d(0.673, 0));
    mapVertex.push_back(cv::Point2d(0.673, 0.575));

    dataCenter.m_mapVertex = vertex;

    Image2World image2World;
    image2World.getTransMat(vertex, mapVertex, dataCenter.m_transMatrix);
    dataCenter.saveImageTransform();

    cv::Mat finalImage;
    cv::namedWindow("final", cv::WINDOW_KEEPRATIO);
    undistorImage.copyTo(finalImage);
    running = true;
    while(running)
    {
    	camera >> rawImage;
    	cv::undistort(rawImage, undistorImage, dataCenter.m_cameraMatrix, dataCenter.m_distCoeffs);
    	cv::warpPerspective(undistorImage, finalImage, dataCenter.m_transMatrix, cv::Size(1340, 1140));

    	cv::imshow("final", finalImage);
    	button = cv::waitKey(1);
    	if(button == 113 || button == 10)
    	{
    		running = false;
    	}
    }

    return 0;
}
