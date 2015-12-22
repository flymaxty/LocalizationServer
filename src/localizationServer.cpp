#include <iostream>
#include <opencv2/opencv.hpp>

const std::string keys =
    "{help h    |   |print help}"
    "{image     |   |source image}"
    ;


bool getRawMap(cv::Mat& in_rawImage, cv::Mat& in_mapRawImage)
{
    cv::Mat grayImage, thresImage, cannyImage;
    cv::Mat redImage;
    cv::inRange(in_rawImage, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 255), redImage);

    //cv::cvtColor(in_rawImage, grayImage, cv::COLOR_BGR2GRAY);
    //adaptiveThreshold(grayImage, thresImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,201,20);
    //cv::Canny(thresImage, cannyImage, 150, 200);

}
int main(int argc, char** argv)
{
    cv::CommandLineParser parser(argc, argv, keys);
    parser.about("LocalizationServer v0.1.0");

    if(parser.has("help"))
    {
        parser.printMessage();
        return 0;
    }

    std::string imageName = parser.get<std::string>("image");
    std::cout << "Image Name: " << imageName << std::endl;
    cv::Mat rawImage = cv::imread(imageName);
    cv::Mat outputImage(rawImage);

    cv::Mat redImage;
    cv::inRange(rawImage, cv::Scalar(0, 0, 0), cv::Scalar(0, 0, 255), redImage);

    cv::Mat channels[3];
    cv::split(rawImage, channels);

    imshow("1", channels[0]);
    imshow("2", channels[1]);
    imshow("3", channels[2]);

    cv::cvtColor(rawImage, rawImage, cv::COLOR_BGR2GRAY);
    cv::Mat thresImage;
    adaptiveThreshold(rawImage, thresImage, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY,201,20);
    cv::Mat cannyImage;
    cv::Canny(rawImage, cannyImage, 150, 200);

    std::vector<cv::Vec3f> circles;
    cv::HoughCircles(cannyImage, circles, cv::HOUGH_GRADIENT, 2, cannyImage.rows / 2, 30, 15, 0, 150);

    for(uint16_t i=0; i < circles.size(); i++)
    {
        cv::circle(outputImage, cv::Point2f(circles[i][0], circles[i][1]), circles[i][2],
            cv::Scalar( 255, 0, 255 ), 4, cv::LINE_AA, 0);
    }

    cv::imshow("cannyImage", cannyImage);
    cv::imshow("rawImage", rawImage);
    cv::imshow("outputImage", outputImage);
    cv::imshow("redImage", redImage);
    cv::waitKey(0);

    return 0;
}