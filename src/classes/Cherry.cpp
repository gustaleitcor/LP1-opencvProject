#include "Cherry.h"
#include <cstdlib>
#include <iostream>

Cherry::Cherry()
{
    setPos(0, 0);
}

Cherry::Cherry(double x, double y)
{
    setPos(x, y);
}

void Cherry::setPos(double x, double y)
{
    this->pos.x = x;
    this->pos.y = y;
}

void Cherry::setNewPos(int bordaX, int bordaY, int size, int scale)
{
    static int aleatoryFactor = 1;
    srand(time(NULL) + aleatoryFactor++);

    int randX = rand();
    int randY = rand();

    setPos((50 + (randX % (bordaX - 100 - size))) / (scale), (50 + (randY % (bordaY - 100 - size))) / (scale));
}