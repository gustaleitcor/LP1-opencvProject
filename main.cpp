#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "drawFunctions.h"
#include "src/classes/Vetor.h"
#include "src/classes/Player.h"
#include "src/classes/Fantasma.h"
#include "src/fps.h"
#include <iostream>

using namespace std;
using namespace cv;

string cascadeName;

int main(int argc, const char **argv)
{
    // Video variveis
    VideoCapture capture;
    Mat frame;
    bool tryflip;
    CascadeClassifier cascade;
    double scale;
    vector<Rect> faces;
    Scalar color = Scalar(255, 0, 0);
    Mat resizedImg = cv::imread("src/imgs/pacman.png", IMREAD_UNCHANGED);
    // FPS variaveis
    double fps = 0;
    double startTime = cv::getTickCount();
    int frameCount = 0;
    // Player variaveis
    Player player(0, 0, 1);
    double modulo;
    Vetor posUnit;
    int faceWidth, faceHeight;
    double pacmanScale = 1;
    Mat pacman_img = cv::imread("src/imgs/pacman.png", IMREAD_UNCHANGED);

    scale = 3; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = false;

    cascadeName = "haarcascade_frontalface_default.xml";

    if (!cascade.load(cascadeName))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

    if (!capture.open("rtsp://192.168.0.7:8080/h264_pcm.sdp")) // para testar com um video
    // if(!capture.open(0))
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    if (capture.isOpened())
    {

        cout << "Video capturing has been started ..." << endl;

        player.pos.setCoordenadas(frame.cols / 2.0, frame.rows / 2.0, 0);

        while (true)
        {
            capture >> frame;
            if (frame.empty())
                break;

            faces = detectFaces(frame, cascade, scale, tryflip);

            if (faces.size() > 0)
            {
                Rect r = faces[0];

                for (auto face : faces)
                {
                    if (face.width * face.height > r.width * r.height)
                    {
                        r = face;
                    }
                }

                modulo = player.pos.dist(r.width, r.height);
                posUnit.setCoordenadas((r.x - player.pos.x) / modulo, (r.y - player.pos.y) / modulo, 0);
                player.acc.setCoordenadas(posUnit.x * posUnit.x * (r.x - player.pos.x) / abs(r.x - player.pos.x), posUnit.y * posUnit.y * (r.y - player.pos.y) / abs(r.y - player.pos.y), 0);
                player.atualizar();
                player.vel.x *= 0.85;
                player.vel.y *= 0.85;

                // resize(pacman_img, resizedImg, Size(r.width, r.height), INTER_LINEAR);

                rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
                          Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                          color, 3);
            }
            else
            {
            }

            // Desenha o Player
            drawTransparency(frame, resizedImg, player.pos.x + r.width / 2 - resizedImg.cols, player.pos.y + r.height / 2 - resizedImg.rows);
            // Atualiza o fps
            attFPS(fps, frameCount, startTime);
            // Desenha o fps no frame
            putText(frame, std::to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));
            // Desenha o frame na tela
            imshow("Pacman - OpenCV", frame);

            char c = (char)waitKey(10);
            if (c == 27 || c == 'q' || c == 'Q')
                break;
        }
    }

    return 0;
}
