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

    setPos(75 + (randX % (bordaX - 75 - size)), 75 + (randY % (bordaY - 75 - size)));
}