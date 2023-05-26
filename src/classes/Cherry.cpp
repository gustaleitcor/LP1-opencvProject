#include "Cherry.h"
#include <cstdlib>
#include <iostream>

void Cherry::setPos(double x, double y)
{
    this->pos.x = x;
    this->pos.y = y;
}

void Cherry::getNewPos(int bordaX, int bordaY)
{

    srand((unsigned)time(NULL));

    int randX = rand();
    int randY = rand();

    std::cout << randX % bordaX << ' ' << randY % bordaY << std::endl;

    setPos(randX % bordaX, randY % bordaY);
}