#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;

/**
 * @brief Draws a transparent image over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param transp the Mat image with transparency, read from a PNG image, with the IMREAD_UNCHANGED flag
 * @param xPos x position of the frame image where the image will start.
 * @param yPos y position of the frame image where the image will start.
 */
void drawTransparency(Mat frame, Mat transp, int xPos, int yPos)
{
    Mat mask;
    vector<Mat> layers;

    split(transp, layers); // seperate channels
    Mat rgb[3] = {layers[0], layers[1], layers[2]};
    mask = layers[3];      // png's alpha channel used as mask
    merge(rgb, 3, transp); // put together the RGB channels, now transp insn't transparent
    transp.copyTo(frame.rowRange(yPos, yPos + transp.rows).colRange(xPos, xPos + transp.cols), mask);
}

/**
 * @brief Draws a transparent rect over a frame Mat.
 *
 * @param frame the frame where the transparent image will be drawn
 * @param color the color of the rect
 * @param alpha transparence level. 0 is 100% transparent, 1 is opaque.
 * @param regin rect region where the should be positioned
 */
void drawTransRect(Mat frame, Scalar color, double alpha, Rect region)
{
    Mat roi = frame(region);
    Mat rectImg(roi.size(), CV_8UC3, color);
    addWeighted(rectImg, alpha, roi, 1.0 - alpha, 0, roi);
}

vector<Rect> detectFaces(Mat &img, CascadeClassifier &cascade, double scale, bool tryflip)
{
    vector<Rect> faces;
    Mat gray, smallImg;

    if (tryflip)
        flip(img, img, 1);

    cvtColor(img, gray, COLOR_BGR2GRAY);
    double fx = 1 / scale;
    resize(gray, smallImg, Size(), fx, fx, INTER_LINEAR_EXACT);
    if (scale > 1)
        resize(img, img, Size(), fx, fx, INTER_LINEAR_EXACT);
    equalizeHist(smallImg, smallImg);

    // t = (double)getTickCount();

    cascade.detectMultiScale(smallImg, faces,
                             1.3, 2, 0
                                         //|CASCADE_FIND_BIGGEST_OBJECT
                                         //|CASCADE_DO_ROUGH_SEARCH
                                         | CASCADE_SCALE_IMAGE,
                             Size(40, 40));

    return faces;

    /* t = (double)getTickCount() - t;
        printf("detection time = %g ms\n", t * 1000 / getTickFrequency());
        PERCORRE AS FACES ENCONTRADAS
    for (size_t i = 0; i < faces.size(); i++)
    {
        Rect r = faces[i];
        rectangle(img, Point(cvRound(r.x), cvRound(r.y)),
                  Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                  color, 3);
    }

    // Desenha o frame na tela
    imshow("result", img);
    */
}
