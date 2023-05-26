#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "drawFunctions.h"
#include "src/classes/Vetor.h"
#include "src/classes/Player.h"
#include "src/classes/Fantasma.h"
#include "src/classes/Cherry.h"
#include "src/fps.h"
#include <iostream>
#include <vector>

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

    // FPS variaveis
    double fps = 0;
    double startTime = cv::getTickCount();
    int frameCount = 0;

    // Player variaveis
    Player player(0, 0, 1);
    double norma;
    Vetor posUnit;
    int faceWidth, faceHeight;
    double pacmanScale = 1;
    Mat pacman_resizedImg = cv::imread("src/sprites/pacmein.png", IMREAD_UNCHANGED);
    Mat pacman_img = cv::imread("src/sprites/pacmein.png", IMREAD_UNCHANGED);

    // Fantasmas variaveis
    vector<Fantasma> fantasmas;
    Mat fanta1 = cv::imread("src/sprites/fanta1.png", IMREAD_UNCHANGED);
    Mat fanta1_resized;
    Mat fanta2 = cv::imread("src/sprites/fanta2.png", IMREAD_UNCHANGED);
    Mat fanta2_resized;
    double size_x, size_y;

    // Cherry variables
    Cherry cherry;
    bool spawnCherry = true;
    Mat resizedCherry_img;
    Mat cherry_img = cv::imread("src/sprites/cherry.png", IMREAD_UNCHANGED);

    //  resize(pacman_img, pacman_resizedImg, Size(r.width, r.height), INTER_LINEAR);

    scale = 1; // usar 1, 2, 4.
    if (scale < 1)
        scale = 1;
    tryflip = true;

    cascadeName = "haarcascade_frontalface_default.xml";

    if (!cascade.load(cascadeName))
    {
        cerr << "ERROR: Could not load classifier cascade" << endl;
        return -1;
    }

    // if (!capture.open("rtsp://192.168.0.7:8080/h264_pcm.sdp")) // para testar com um video
    if (!capture.open("video.mp4"))
    {
        cout << "Capture from camera #0 didn't work" << endl;
        return 1;
    }

    if (capture.isOpened())
    {
        capture >> frame;

        cout << "Video capturing has been started ..." << endl;

        player.pos.setCoordenadas(frame.cols / 2.0, frame.rows / 2.0, 0);

        // Definicacao do tamanho e posicao dos fantamas
        size_x = frame.cols / 8;
        size_y = frame.rows / 8;

        fantasmas.push_back(Fantasma(size_x, size_y, 0));
        fantasmas.push_back(Fantasma(frame.cols - size_x, frame.rows - size_y, 0));

        resize(cherry_img, resizedCherry_img, Size(size_x, size_y), INTER_LINEAR);
        while (true)
        {
            capture >> frame;
            if (frame.empty())
                break;

            faces = detectFaces(frame, cascade, scale, tryflip);

            if (faces.size() > 0)
            {
                Rect r = faces[0];

                // loop para resgatar a maior face
                for (auto face : faces)
                {
                    if (face.width * face.height > r.width * r.height)
                    {
                        r = face;
                    }
                }

                // calcula o movimento do jogador
                norma = player.pos.dist(r.width, r.height);
                if (norma > 10)
                {
                    posUnit.setCoordenadas((r.x - player.pos.x) / norma, (r.y - player.pos.y) / norma, 0);
                    player.vel.setCoordenadas(posUnit.x * 10, posUnit.y * 10, 0);
                }
                else
                {
                    player.vel.setCoordenadas(0, 0, 0);
                }
                player.atualizar();

                resize(pacman_img, pacman_resizedImg, Size(r.width, r.height), INTER_LINEAR);

                rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
                          Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                          color, 3);
            }

            // Verifica se é para realeatorizar a posição da Cherry
            if (spawnCherry)
            {
                cherry.getNewPos(frame.cols, frame.rows);
                spawnCherry = false;
            }

            // Resize dos fantamas
            resize(fanta1, fanta1_resized, Size(size_x, size_y), INTER_LINEAR);
            resize(fanta2, fanta2_resized, Size(size_x, size_y), INTER_LINEAR);

            // Desenha a cherry
            drawTransparency(frame, resizedCherry_img, cherry.pos.x, cherry.pos.y);

            // Desenha o Player
            drawTransparency(frame, pacman_resizedImg, player.pos.x, player.pos.y);

            // Atualiza o fps
            attFPS(fps, frameCount, startTime);

            // Desenha o fps no frame
            putText(frame, std::to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));

            // Desenha os fantasmas
            drawTransparency(frame, fanta1_resized, fantasmas[0].pos.x, fantasmas[0].pos.y);
            drawTransparency(frame, fanta2_resized, fantasmas[1].pos.x, fantasmas[1].pos.y);

            // Desenha o frame na tela
            imshow("Pacman - OpenCV", frame);

            char c = (char)waitKey(10);
            if (c == 27 || c == 'q' || c == 'Q')
                break;
        }
    }

    return 0;
}
