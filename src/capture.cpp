#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"

int main(void)
{
	cv::VideoCapture cam(1);
	//cam.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
	//cam.set(cv::CAP_PROP_FRAME_HEIGHT, 768);

	cv::Mat rawImage;
	while(1)
	{
		cam >> rawImage;
		cv::imshow("Image", rawImage);

		if (cv::waitKey(1) != -1)
		{
			std::string imageName;
			std::cout << "Image Name: ";
			std::cin >> imageName;
			imageName ="resource/" + imageName;
			std::cout << imageName << std::endl;
			cv::imwrite(imageName, rawImage);
			break;
		}
	}
	return 0;
}
