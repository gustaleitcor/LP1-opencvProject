#pragma once
#include "Vetor.h"

class Player
{
public:
    Vetor pos;
    Vetor vel;
    Vetor acc;

    Player();
    Player(double x, double y, double z);

    void atualizar();
};