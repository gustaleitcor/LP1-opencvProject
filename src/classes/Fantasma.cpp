#include "Fantasma.h"

Fantasma::Fantasma(){}

Fantasma::Fantasma(double x, double y, double z){
    pos.setCoordenadas(x, y, 0);
    vel.setCoordenadas(10, 10, 0);
}

void Fantasma::atualizar(){
    pos.sum(vel);
}

