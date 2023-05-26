#include "Cherry.h"
#include <cstdlib>
#include <iostream>

void Cherry::setPos(double x, double y)
{
    this->pos.x = x;
    this->pos.y = y;
}

void Cherry::getNewPos(int bordaX, int bordaY, int size)
{

    srand((unsigned)time(NULL));

    int randX = rand();
    int randY = rand();

    setPos(randX % (bordaX - size), randY % (bordaY - size));
}