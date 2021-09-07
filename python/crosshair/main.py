import cv2


class rinCross:
    def __init__(self):
        self.frame = ""

    def drawcross(self, x: int, y: int, width: int, height: int) -> None:
        """Draws a Crosshair on the screen

        Parameters
        -----------
        x: :class:`int`
            X
        y: :class:`int`
            y
        width: :class:`int`
            width
        height: :class:`int`
            height

        Return
        -----------
            None
        """
        centerX = (x + x + width) // 2
        centerY = (y + y + height) // 2

        cv2.line(self.frame, (width, centerY), (0,  centerY), (0, 0, 100), 3)
        cv2.line(self.frame, (centerX, height), (centerX, 0), (0, 0, 100), 3)
        return

    def run(self):
        cap = cv2.VideoCapture(
            "https://5b61f36fddea5.streamlock.net:1937/ultsol/carlos_st.stream/index.m3u8")
        _, self.frame = cap.read()

        while True:

            # Get Frame with and height
            w, h = self.frame.shape[1], self.frame.shape[0]
            self.drawcross(200, 200, w, h)
            # self.drawcross(600, 5, w, h)

            cv2.imshow('Frame', self.frame)

            k = cv2.waitKey(1) & 0xFF
            if k == 27:  # Press the ESC key to exit
                break

        cv2.destroyAllWindows()


if __name__ == "__main__":
    rinCross().run()
