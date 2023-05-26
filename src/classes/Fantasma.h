#pragma once
#include "Vetor.h"

class Fantasma
{
public:
    Vetor pos;
    Vetor vel;
    Vetor acc;

    Fantasma();
    Fantasma(double x, double y, double z);

    void atualizar();
};