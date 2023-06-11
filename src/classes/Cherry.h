#pragma once
#include "Vetor.h"
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"

using namespace std;
using namespace cv;

class Cherry
{
public:
    Vetor pos;

    Cherry();
    Cherry(double x, double y);
    void setPos(double x, double y);
    void setNewPos(int bordaX, int bordaY, int size, int scale);
};