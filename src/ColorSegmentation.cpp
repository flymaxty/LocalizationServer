/*
 * ColorSegmentation.cpp
 *
 *  Created on: Dec 23, 2015
 *      Author: ye
 */

#include "ColorSegmentation.h"

ColorSegmentation::ColorSegmentation()
{
    // TODO Auto-generated constructor stub

}

ColorSegmentation::~ColorSegmentation()
{
    // TODO Auto-generated destructor stub
}

bool ColorSegmentation::setThreshold(cv::Scalar minValue, cv::Scalar maxValue)
{
    m_minValue = minValue;
    m_maxValue = maxValue;
}

void ColorSegmentation::drawPoints(cv::InputOutputArray in_image, cv::Scalar in_color)
{
    cv::Mat image = in_image.getMat();
    uint8_t length = 5;
    for(uint16_t i=0; i<m_points.size(); i++)
    {
        cv::line(image, cv::Point(m_points[i].x-length, m_points[i].y),
            cv::Point(m_points[i].x+length, m_points[i].y), in_color, 1, cv::LINE_AA, 0);
        cv::line(image, cv::Point(m_points[i].x, m_points[i].y-length),
            cv::Point(m_points[i].x, m_points[i].y+length), in_color, 1, cv::LINE_AA, 0);
    }
}

void ColorSegmentation::drawContours(cv::InputOutputArray in_image, cv::Scalar in_color)
{
    cv::Mat image = in_image.getMat();

    for(uint16_t i=0; i<m_contours.size(); i++)
    {
        cv::drawContours(image, m_contours, i, in_color, 1, cv::LINE_AA);
    }
}

bool ColorSegmentation::getBlocks(cv::InputArray in_image, std::vector<cv::Point2d>& in_points)
{
    cv::Mat rawImage = in_image.getMat();

    cv::cvtColor(rawImage, m_yuvImage, cv::COLOR_BGR2YUV);
    cv::inRange(m_yuvImage, m_minValue, m_maxValue, m_thresholdImage);

    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT,
                                                cv::Size( 5, 5 ), cv::Point(2, 2));


    cv::morphologyEx(m_thresholdImage, m_filterImage, cv::MORPH_OPEN, element);
    cv::Mat filterImage = m_filterImage.clone();
    cv::findContours(filterImage, m_contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

    cv::Moments mnt;
    double cx, cy;
    in_points.clear();
    if(m_contours.size() < 50)
    {
        for(uint16_t i=0; i<m_contours.size(); i++)
        {
            mnt = cv::moments(m_contours[i]);
            cx = mnt.m10 / mnt.m00;
            cy = mnt.m01 / mnt.m00;
            in_points.push_back(cv::Point2d(cx, cy));
        }
    }

    m_points = in_points;

    return true;
}
