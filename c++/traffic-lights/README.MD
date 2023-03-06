# Yume Trafic Light Dection

The code is a C++ program that uses OpenCV library to detect traffic lights in a video stream captured from a specified URL. The program first includes the necessary headers and namespaces, and then creates a VideoCapture object to read frames from the stream.

The inRange function is defined to check if a given number is within a specified range. Then, the main loop iterates over the frames captured by the VideoCapture object.

For each frame, a rectangular region of interest (ROI) is cropped from the frame using OpenCV's Range function. The cv::medianBlur function is applied to the cropped image to remove noise, and the cropped image is converted from BGR color space to HSV color space using cv::cvtColor. A binary image is generated using cv::inRange to detect the red color range in the cropped image.

Contours are detected in the binary image using cv::findContours, and each contour is checked for its area. If the area is greater than a specified threshold, the contour is assumed to be the traffic light and its bounding rectangle is obtained using cv::boundingRect. The bounding rectangle's coordinates are used to draw a rectangle around the detected traffic light in the original frame using cv::rectangle.

The program then checks the y-coordinate of the bounding rectangle to determine the color of the traffic light. If the y-coordinate is within a certain range, the light is assumed to be red, green or yellow based on the position of the y-coordinate in the range. The color of the traffic light is then displayed on the frame using cv::putText.

Finally, the program displays the frame with the detected traffic light and waits for the user to press the 'q' key or the 'Esc' key to exit. Once the loop exits, the VideoCapture object is released and all windows are destroyed before the program terminates.
