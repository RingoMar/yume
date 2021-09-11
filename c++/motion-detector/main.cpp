/**
    Yume Motion Dection: C++ opencv
    @author RingoMar
    @version 1.0 10/09/21
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

/**
    Draws region 1 box
    @param frame - Frame to draw on
    @return void
*/
void region_1(cv::Mat frame)
{
    int wid = capture.get(CAP_PROP_FRAME_WIDTH) / 3;

    Point pt1, pt2;
    pt1.x = 0 + 2;
    pt1.y = 0 + 2;

    pt2.x = pt1.x + wid - 4;
    pt2.y = pt1.y + capture.get(CAP_PROP_FRAME_HEIGHT) - 4;

    rectangle(frame, pt1, pt2, CV_RGB(255, 0, 0), 3);
}

/**
    Draws region 2 box
    @param frame - Frame to draw on
    @return void
*/
void region_2(cv::Mat frame)
{
    int wid = capture.get(CAP_PROP_FRAME_WIDTH) / 3;

    Point pt1, pt2;
    pt1.x = capture.get(CAP_PROP_FRAME_WIDTH) / 3 + 2;
    pt1.y = 0 + 2;

    pt2.x = pt1.x + wid - 4;
    pt2.y = pt1.y + capture.get(CAP_PROP_FRAME_HEIGHT) - 4;

    rectangle(frame, pt1, pt2, CV_RGB(0, 255, 0), 3);
}

/**
    Draws region 3 box
    @param frame - Frame to draw on
    @return void
*/
void region_3(cv::Mat frame)
{
    int wid = capture.get(CAP_PROP_FRAME_WIDTH) / 3 + capture.get(CAP_PROP_FRAME_WIDTH) / 3;

    Point pt1, pt2;
    pt1.x = capture.get(CAP_PROP_FRAME_WIDTH) / 3 + capture.get(CAP_PROP_FRAME_WIDTH) / 3 + 4;
    pt1.y = 0 + 2;

    pt2.x = pt1.x + wid - 4;
    pt2.y = pt1.y + capture.get(CAP_PROP_FRAME_HEIGHT) - 4;

    rectangle(frame, pt1, pt2, CV_RGB(0, 0, 255), 3);
}

int main()
{

    std::cout << "\n\nYume a Computer Vision project using OpenCV\n\n"
              << " _\n"
              << "| |\n"
              << " _ " << endl;
    Ptr<BackgroundSubtractor> pBackSub;
    pBackSub = cv::createBackgroundSubtractorKNN(50, 900, false);
    Mat frame, fgMask;

    while (true)
    {
        capture >> frame;
        if (frame.empty())
            break;

        //                               y,		y+h				 x,		x+w
        Mat cropped_image = frame(Range(203, 203 + 401), Range(129, 129 + 1079));

        Mat thresh;
        cvtColor(cropped_image, cropped_image, COLOR_BGR2GRAY);
        pBackSub->apply(cropped_image, thresh);
        threshold(thresh, thresh, 150, 255, THRESH_BINARY);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresh, contours, RETR_LIST, CHAIN_APPROX_NONE);

        for (int i = 0; i < contours.size(); i++)
        {
            double area = contourArea(contours[i]);
            if (area > 50)
            {
                // cv::drawContours(frame, contours, i, cv::Scalar(0, 0, 0), -1); // Uncomment to show mask

                Rect rect = boundingRect(contours[i]);
                Point pt1, pt2;
                pt1.x = rect.x + 129;
                pt1.y = rect.y + 203;

                pt2.x = pt1.x + rect.width;
                pt2.y = pt1.y + rect.height;

                // rectangle(frame, pt1, pt2, CV_RGB(255, 0, 255), 3); // Uncomment to show individual dection

                if (inRange(0, capture.get(CAP_PROP_FRAME_WIDTH) / 3, rect.x))
                {
                    region_1(frame);
                }
                else if (inRange(capture.get(CAP_PROP_FRAME_WIDTH) / 3, capture.get(CAP_PROP_FRAME_WIDTH) / 3 + capture.get(CAP_PROP_FRAME_WIDTH) / 3, rect.x))
                {
                    region_2(frame);
                }
                else
                {
                    region_3(frame);
                }
            }
        }

        imshow("Yume Motion Dection ", frame);
        imshow("Mask Data", thresh);

        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
    }
    capture.release();
    destroyAllWindows();
    return 0;
}