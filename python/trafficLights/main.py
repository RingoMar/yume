import cv2
import numpy as np


def draw_str(dst, target: tuple, s: str) -> None:
    """Quickly draw a string on the frame using white on black to show up no matter what is on the screen.

    Parameters
    -----------
    dst: :class:`Image`
        The frame to put on.
    target: :class:`tuple`
        Location of the text.
    s: :class:`str`
        Text you want on the page..

    Return
    -----------
        NONE
    """

    x, y = target
    cv2.putText(dst, s, (x+1, y+1), cv2.FONT_HERSHEY_PLAIN, 1.0, (0, 0, 0), thickness=2, lineType=cv2.LINE_AA)
    cv2.putText(dst, s, (x, y), cv2.FONT_HERSHEY_PLAIN, 1.0, (255, 255, 255), lineType=cv2.LINE_AA)


def main():
    vid = cv2.VideoCapture(
        "https://5b61f36fddea5.streamlock.net:1937/ultsol/carlos_st.stream/index.m3u8")
    boxes_avg = {}
    while True:
        _, frame = vid.read()
        detections = []
        height, widith, _ = frame.shape
        hsv = cv2.cvtColor(frame[81:81+86, 992:992+75], cv2.COLOR_BGR2HSV) # Image Crop:               y:y+h,   x:x+w
        # hsv = cv2.cvtColor( frame[185:185+114, 27:27+71], cv2.COLOR_BGR2HSV) #left side light for testing
        _height, _, _ = hsv.shape

        lower_range = np.array([0, 0, 0])
        upper_range = np.array([250, 250, 250])

        medBlur = cv2.medianBlur(hsv, 3)
        mask = cv2.inRange(medBlur, lower_range, upper_range)

        contours, _ = cv2.findContours(mask, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
        for cnt in contours:
            area = cv2.contourArea(cnt)
            # Average area size of the light and ignoring errors like cars/trucks reflecting light.
            if area < 1000 and area > 40:
                x, y, w, h = cv2.boundingRect(cnt)
                cx = (x + x + w) // 2
                cy = (y + y + h) // 2
                detections.append([cx, cy])

                x = x + 992  # Add back the crop to the orgingal for better drawing on frame
                y = y + 81  # Add back the crop to the orgingal for better drawing on frame
                cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 3)
                draw_str(frame, (20, height - 60), f"Light Pos:   {cy}")
                draw_str(frame, (20, height - 50), "-------------")

                if str(cy) not in boxes_avg:
                    boxes_avg[str(cy)] = 1
                else:
                    boxes_avg[str(cy)] += 1

        for box in detections:
            upper, middle, bottom = (0, int(_height / 3 + 10)), (int(_height / 3) - 3, int(_height / 3 + _height / 3)
                                         + 5),(int(_height / 3 + _height / 3) - 3, int(_height / 3 + _height / 3 + _height / 3) + 5)
            print(upper, middle, bottom, "-> ", box[1])

            if box[1] in range(upper[0], upper[1]):
                draw_str(frame, (20, height - 30), "Left Light:  " + "GREEN")
                draw_str(frame, (20, height - 10), "Right Light: " + "Red")
            elif box[1] in range(middle[0], middle[1]):
                draw_str(frame, (20, height - 30), "Left Light:  " + "RED")
                draw_str(frame, (20, height - 10), "Right Light: " + "Yellow")
            elif box[1] in range(bottom[0], bottom[1]):
                draw_str(frame, (20, height - 30), "Left Light:  " + "RED")
                draw_str(frame, (20, height - 10), "Right Light: " + "GREEN")
            else:
                draw_str(frame, (20, height - 30), "Left Light:  " + "UNKNOWN")
                draw_str(frame, (20, height - 10), "Right Light: " + "UNKNOWN")

        cv2.imshow('Light Detector', frame)
        # cv2.imshow('mask Light Detect', mask)

        key = cv2.waitKey(30)
        if key == 27:
            vid.release()
            cv2.destroyAllWindows()
            break

    print("Average placements of the lights for hard coding:", dict(sorted(boxes_avg.items(), key=lambda x: x[1])))
    vid.release()
    cv2.destroyAllWindows()


if __name__ == "__main__":
    main()
