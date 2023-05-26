#include "Cherry.h"
#include <random>

void Cherry::getNewPos(int bordaX, int bordaY)
{
    double randX = std::rand();
    double randY = std::rand();

    this->pos.x = randX * bordaX;
    this->pos.y = randY * bordaY;
}