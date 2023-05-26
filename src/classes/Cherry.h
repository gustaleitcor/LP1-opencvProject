#pragma once
#include "Vetor.h"

class Cherry
{
public:
    Vetor pos;

    void setPos(double x, double y);
    void getNewPos(int bordaX, int bordaY);
}