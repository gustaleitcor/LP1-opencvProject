#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include "drawFunctions.h"
#include "src/classes/Vetor.h"
#include "src/classes/Player.h"
#include "src/classes/Fantasma.h"
#include "src/classes/Cherry.h"
#include "src/classes/PerlinNoise.h"
#include "src/funcoes/dist.h"
#include "src/fps.h"
#include <iostream>
#include <vector>
#include <time.h>
#include <stdlib.h>

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
    Vetor resizar;

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
    resize(pacman_img, pacman_resizedImg, Size(150, 150), INTER_LINEAR);
    bool firstFrame = true;

    // Fantasmas variaveis
    vector<Fantasma> fantasmas;
    Mat fanta1 = cv::imread("src/sprites/fanta1.png", IMREAD_UNCHANGED);
    Mat fanta1_resized;
    Mat fanta2 = cv::imread("src/sprites/fanta2.png", IMREAD_UNCHANGED);
    Mat fanta2_resized;
    PerlinNoise pn = PerlinNoise(time(NULL));

    // Cherry variables
    Cherry cherry;
    int points = 0;
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
    if (!capture.open("loira.mp4"))
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
        resizar = resizeFactor(frame.cols, frame.rows, 8);

        fantasmas.push_back(Fantasma(resizar.x, resizar.y, 0));
        fantasmas.push_back(Fantasma(frame.cols - resizar.x * 2, frame.rows - resizar.y * 2, 0));

        resize(cherry_img, resizedCherry_img, Size(resizar.x, resizar.y), INTER_LINEAR);

        cherry.getNewPos(frame.cols, frame.rows, (int)resizar.x);

        srand(time(NULL));

        fantasmas[0].vel.setCoordenadas(((rand() % 2) - 0.5) * 2 * 10, ((rand() % 2) - 0.5) * 2 * 10, 0);

        // variveis menu

        // Carrega play button
        Mat play_button = imread("src/sprites/play_button.png", IMREAD_UNCHANGED);
        resize(play_button, play_button, Size(frame.cols / 8, frame.cols / 8), INTER_LINEAR);
        // Carrega exit button
        Mat exit_button = imread("src/sprites/exit_button.png", IMREAD_UNCHANGED);
        resize(exit_button, exit_button, Size(frame.cols / 8, frame.cols / 8), INTER_LINEAR);

        // menu
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

                rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
                          Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                          color, 3);

                // Botão para jogar
                double dist_play = dist(r.x + r.width / 2, r.y + r.height / 2, frame.cols - (frame.cols / 14) - play_button.cols, frame.rows / 2 - play_button.rows / 2);
                double dist_exit = dist(r.x + r.width / 2, r.y + r.height / 2, frame.cols / 14, frame.rows / 2 - play_button.rows / 2);

                std::cout << "Dist play " << dist_play << std::endl;
                std::cout << "Dist exit " << dist_exit << std::endl;
                if (dist_play < 80)
                {
                    break;
                }

                if (dist_exit < 80)
                {
                    return 0;
                }
            }

            // getTextSize("Test", font, fontScale, thickness, nullptr);

            // Desenha o nome menu
            putText(frame, "Menu", Point(frame.cols / 2 - 155, 90), FONT_HERSHEY_PLAIN, 5, Scalar(255, 0, 0), 5);

            // Desenha o recorde
            putText(frame, "Recorde atual: ", Point(frame.cols / 2 - 400, frame.rows - 50), FONT_HERSHEY_PLAIN, 5, Scalar(240, 32, 160), 5);

            // Desenha o play button
            drawTransparency(frame, play_button, frame.cols - (frame.cols / 14) - play_button.cols, frame.rows / 2 - play_button.rows / 2);
            // Desenha o exit button
            drawTransparency(frame, exit_button, frame.cols / 14, frame.rows / 2 - play_button.rows / 2);

            imshow("Pacman - OpenCV", frame);

            char c = (char)waitKey(10);
            if (c == 27 || c == 'q' || c == 'Q')
                break;
        }

        // jogo
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
                if (norma > 50)
                {
                    posUnit.setCoordenadas((r.x + r.width / 2 - player.pos.x - 75) / norma, (r.y + r.height / 2 - player.pos.y - 75) / norma, 0);
                    player.vel.setCoordenadas(posUnit.x * 50, posUnit.y * 50, 0);
                }
                else
                {
                    player.vel.setCoordenadas(0, 0, 0);
                }
                player.atualizar();
                firstFrame = false;

                /*rectangle(frame, Point(cvRound(r.x), cvRound(r.y)),
                          Point(cvRound((r.x + r.width - 1)), cvRound((r.y + r.height - 1))),
                          color, 3);*/
            }

            // movimento fantasma de movimento aleatorio 0<->1 -1<->1  (0-0.5) * 2

            if (fantasmas[0].pos.x + fanta1_resized.cols + fantasmas[0].vel.x > frame.cols || fantasmas[0].pos.x + fantasmas[0].vel.x < 0)
            {
                fantasmas[0].vel.x *= -1;
            }
            if (fantasmas[0].pos.y + fanta1_resized.rows + fantasmas[0].vel.y > frame.rows || fantasmas[0].pos.y + fantasmas[0].vel.y < 0)
            {
                fantasmas[0].vel.y *= -1;
            }

            fantasmas[0].atualizar();

            // movimento fantasma que segue
            norma = fantasmas[1].pos.dist(player.pos.x, player.pos.y);
            if (norma > 2)
            {
                posUnit.setCoordenadas((player.pos.x - fantasmas[1].pos.x) / norma, (player.pos.y - fantasmas[1].pos.y) / norma, 0);
                fantasmas[1].vel.setCoordenadas(posUnit.x * 2, posUnit.y * 2, 0);
            }
            else
            {
                fantasmas[1].vel.setCoordenadas(0, 0, 0);
            }
            fantasmas[1].atualizar();

            // Aumento de pontos quando comer cherry
            norma = cherry.pos.dist(player.pos.x, player.pos.y);
            if (norma <= pacman_resizedImg.cols / 2)
            {
                cherry.getNewPos(frame.cols, frame.rows, (int)resizar.x);
                points++;
            }

            // Verifica se é para realeatorizar a posição da Cherry

            // Resize dos fantamas
            resize(fanta1, fanta1_resized, Size(resizar.x, resizar.y), INTER_LINEAR);
            resize(fanta2, fanta2_resized, Size(resizar.x, resizar.y), INTER_LINEAR);

            // Desenha a cherry
            drawTransparency(frame, resizedCherry_img, cherry.pos.x, cherry.pos.y);

            // Desenha o Player
            if (!firstFrame)
            {
                drawTransparency(frame, pacman_resizedImg, player.pos.x, player.pos.y);
            }

            // Atualiza o fps
            attFPS(fps, frameCount, startTime);

            // Desenha o fps no frame
            putText(frame, std::to_string(fps), Point(5, 15), FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 0));

            // Desenha os fantasmas
            drawTransparency(frame, fanta1_resized, fantasmas[0].pos.x, fantasmas[0].pos.y);
            drawTransparency(frame, fanta2_resized, fantasmas[1].pos.x, fantasmas[1].pos.y);

            // Desenha placar
            putText(frame, "Placar: " + std::to_string(points), Point(frame.cols / 2 - 200, 56), FONT_HERSHEY_PLAIN, 5, Scalar(255, 0, 0), 5);

            // Desenha o frame na tela
            imshow("Pacman - OpenCV", frame);

            char c = (char)waitKey(10);
            if (c == 27 || c == 'q' || c == 'Q')
                break;
        }
    }

    return 0;
}
