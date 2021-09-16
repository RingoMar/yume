/**
    Yume Trafic Light Dection: C++ opencv
    @author RingoMar
    @version 1.0 15/09/21
*/
#include <iostream>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

VideoCapture capture("https://5b61f36fddea5.streamlock.net:1937/ultsol/carlos_st.stream/index.m3u8");
/**
	Tells if the number is in a range
	@param Low - The start of the range
	@param Low - The start of the range
	@param x - The value is True
	@return Bool
*/
bool inRange(unsigned low, unsigned high, unsigned x)
{
	return (low <= x && x <= high);
}

int main()
{

	Ptr<BackgroundSubtractor> pBackSub;
	pBackSub = createBackgroundSubtractorKNN();
	Mat frame, fgMask;

	while (true)
	{
		capture >> frame;
		if (frame.empty())
			break;

		//          y:y+h, x:x+w
		Mat cropped_image = frame(Range(108 - 32, 116 - 32 + 108), Range(987, 987 + 84));
		cv::Size sz = cropped_image.size();
		int imageHeight = sz.width, imageWidth = sz.height;

		cv::Mat orig_image = frame.clone();

		Mat lookUpTable(1, 256, CV_8U);
		uchar *p = lookUpTable.ptr();
		for (int i = 0; i < 256; ++i)
			p[i] = saturate_cast<uchar>(pow(i / 255.0, 3.0) * 255.0);
		Mat res = cropped_image.clone();
		LUT(cropped_image, lookUpTable, res);

		cv::medianBlur(cropped_image, cropped_image, 3);

		cv::Mat hsv_image;
		cv::cvtColor(cropped_image, hsv_image, cv::COLOR_BGR2HSV);

		cv::Mat lower_red_hue_range;
		cv::Mat upper_red_hue_range;
		cv::inRange(hsv_image, cv::Scalar(0, 0, 0), cv::Scalar(250, 250, 250), lower_red_hue_range);

		Mat thresh;
		Mat img_gray;
		cvtColor(hsv_image, img_gray, COLOR_BGR2GRAY);

		threshold(img_gray, thresh, 150, 255, THRESH_BINARY);

		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(thresh, contours, RETR_LIST, CHAIN_APPROX_NONE);

		for (int i = 0; i < contours.size(); i++)
		{
			double area = contourArea(contours[i]);

			if (area > 40)
			{
				//cv::drawContours(frame, contours, i, cv::Scalar(0, 255, 0), -1);

				Rect rect = boundingRect(contours[i]);
				Point pt1, pt2, pt3;
				int maskHeight = 108;

				pt1.x = rect.x + 987;
				pt1.y = rect.y + 108 - 32;

				pt2.x = pt1.x + rect.width;
				pt2.y = pt1.y + rect.height;

				int cx = floor((rect.x + rect.x + rect.width));
				int cy = floor((rect.y + rect.y + rect.height));
				putText(frame, std::to_string(rect.y), cv::Point(19, frame.rows - 39), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(0,0,0), 2);
				putText(frame, std::to_string(rect.y), cv::Point(20, frame.rows - 40), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);

				if (inRange(0, maskHeight / 3 - 5, rect.y))
				{

					putText(frame, "Light: Red?", cv::Point(19, frame.rows - 19), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 0), 2);
					putText(frame, "Light: Red?", cv::Point(20, frame.rows - 20), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
				}
				else if (inRange(maskHeight / 3 + maskHeight / 3 - 10, rect.y, rect.y))
				{

					putText(frame, "Light: Green?", cv::Point(19, frame.rows - 19), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 0), 2);
					putText(frame, "Light: Green?", cv::Point(20, frame.rows - 20), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
				}
				else
				{
					putText(frame, "Light: Yellow?", cv::Point(19, frame.rows - 19), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(0, 0, 0), 2);
					putText(frame, "Light: Yellow?", cv::Point(20, frame.rows - 20), cv::FONT_HERSHEY_PLAIN, 1.0, Scalar(255, 255, 255), 1);
				}
				rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 3);
			}
		}
		imshow("Trafic Light Dection", frame);
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}
	capture.release();
	destroyAllWindows();
	return 0;
}