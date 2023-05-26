#include "Fantasma.h"

Fantasma::Fantasma(){}

Fantasma::Fantasma(double x, double y){
    pos.setCoordenadas(x, y, 0);
    vel.setCoordenadas(10, 10, 0);
}