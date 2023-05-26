#include "Cherry.h"
#include <random>

void Cherry::setPos(double x, double y)
{
    this->pos.x = x;
    this->pos.y = y;
}

void Cherry::getNewPos(int bordaX, int bordaY)
{
    double randX = std::rand();
    double randY = std::rand();

    setPos(randX * bordaX, randY * bordaY);
}