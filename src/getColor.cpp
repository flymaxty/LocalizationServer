#include <iostream>
//#include <sstream>
#include <opencv2/opencv.hpp>

const std::string keys =
    "{help h    |   |print help}"
    "{image     |   |source image}"
    ;

cv::Mat rawImage, tempImage, outputImage;
cv::Mat yuvImage, thresholdImage, filterImage, openImage, closeImage;

cv::FileStorage fs;

int colorNumb = 0;
std::string colorState = "Team Red";

void onMouse(int event, int x, int y, int flags, void* in_param)
{
    int rValue, gValue, bValue;
    int yValue, uValue, vValue;

    if(y >= 10 && y <= 263 && x >= 10 && x <= 394)
    {
		yValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[0];
		uValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[1];
		vValue = yuvImage.at<cv::Vec3b>(y-10, x-10)[2];

		std::ostringstream yuvTextStream;
		yuvTextStream << "Y: " << yValue << " U: " << uValue << " V: " << vValue;
		std::string yuvText = yuvTextStream.str();

		rawImage.copyTo(tempImage);

		int deltaYValue = yValue*0.20;
		int deltaUValue = uValue*0.20;
		int deltaVValue = vValue*0.20;
		cv::inRange(yuvImage, cv::Scalar(yValue - deltaYValue, uValue - deltaUValue, vValue - deltaVValue),
			cv::Scalar(yValue + deltaYValue, uValue + deltaUValue, vValue + deltaVValue), thresholdImage);

		cv::putText(tempImage, yuvText, cv::Point(0,230),
				cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 1.5, cv::LINE_AA);
		cv::putText(tempImage, colorState, cv::Point(0,248),
				cv::FONT_HERSHEY_SIMPLEX, 0.7, cv::Scalar(0, 0, 255), 1.5, cv::LINE_AA);

		if(event == cv::EVENT_LBUTTONUP && colorNumb < 4)
		{
			if(colorNumb == 0)
			{
				//fs << "#Team Red thresold";
				fs << "TeamRedMin" << cv::Scalar(yValue - deltaYValue, uValue - deltaUValue, vValue - deltaVValue);
				fs << "TeamRedMax" << cv::Scalar(yValue + deltaYValue, uValue + deltaUValue, vValue + deltaVValue);
				colorState = "Team Green";
			}
			if(colorNumb == 1)
			{
				fs << "TeamGreenMin" << cv::Scalar(yValue - deltaYValue, uValue - deltaUValue, vValue - deltaVValue);
				fs << "TeamGreenMax" << cv::Scalar(yValue + deltaYValue, uValue + deltaUValue, vValue + deltaVValue);
				colorState = "Number One";
			}
			if(colorNumb == 2)
			{
				fs << "TeamNumb1Min" << cv::Scalar(yValue - deltaYValue, uValue - deltaUValue, vValue - deltaVValue);
				fs << "TeamNumb1Max" << cv::Scalar(yValue + deltaYValue, uValue + deltaUValue, vValue + deltaVValue);
				colorState = "Number Two";
			}

			if(colorNumb == 3)
			{
				fs << "TeamNumb2Min" << cv::Scalar(yValue - deltaYValue, uValue - deltaUValue, vValue - deltaVValue);
				fs << "TeamNumb2Max" << cv::Scalar(yValue + deltaYValue, uValue + deltaUValue, vValue + deltaVValue);
				colorState = "All Done";
				fs.release();
			}

			colorNumb++;
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
    rawImage.copyTo(tempImage);

    thresholdImage = cv::Mat::zeros(rawImage.size(), CV_8UC1);

    cv::cvtColor(rawImage, yuvImage, cv::COLOR_BGR2YUV);

    cv::namedWindow("BigImage");
    cv::setMouseCallback("BigImage", onMouse, NULL);

    std::vector<std::vector<cv::Point> > contours;

    cv::Mat element = cv::getStructuringElement( cv::MORPH_RECT,
                           cv::Size( 5, 5 ), cv::Point(2, 2));

	cv::Moments mnt;
	int cx, cy;

	cv::Mat bigImage(536, 798, rawImage.type());
	cv::Rect image1(10, 10, 384, 253);
	cv::Rect image2(404, 10, 384, 253);
	cv::Rect image3(10, 273, 384, 253);
	cv::Rect image4(404, 273, 384, 253);


    while(1)
    {
    	rawImage.copyTo(outputImage);
    	cv::morphologyEx(thresholdImage, openImage, cv::MORPH_OPEN, element);
    	//cv::morphologyEx(openImage, closeImage, cv::MORPH_CLOSE, element);
    	openImage.copyTo(filterImage);
        cv::findContours(filterImage, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);
        if(contours.size() < 50)
        {
            for(uint16_t i=0; i<contours.size(); i++)
            {
            	cv::drawContours(outputImage, contours, i, cv::Scalar(0,255,255), 1);
            	mnt = cv::moments(contours[i]);
            	cx = int(mnt.m10/mnt.m00);
            	cy = int(mnt.m01/mnt.m00);
            	cv::line(outputImage, cv::Point(cx-5, cy), cv::Point(cx+5, cy),
            			cv::Scalar(0, 0, 255), 1, cv::LINE_AA, 0);
            	cv::line(outputImage, cv::Point(cx, cy-5), cv::Point(cx, cy+5),
            			cv::Scalar(0, 0, 255), 1, cv::LINE_AA, 0);
            }
        }

        tempImage.copyTo(bigImage(image1));
        cv::cvtColor(thresholdImage, bigImage(image2), cv::COLOR_GRAY2BGR);
        cv::cvtColor(openImage, bigImage(image4), cv::COLOR_GRAY2BGR);
        outputImage.copyTo(bigImage(image3));
        cv::imshow("BigImage", bigImage);

        cv::waitKey(1);
    }

    return 0;
}
