#include "Vetor.h"
#include "math.h"

double Vetor::dist(Vetor v)
{
    return sqrt((this->x - v.x) * (this->x - v.x) + (this->y - v.y) * (this->y - v.y) + (this->z - v.z) * (this->z - v.z));
}

double Vetor::dist(double x, double y, double z)
{
    return sqrt((this->x - x) * (this->x - x) + (this->y - y) * (this->y - y) + (this->z - z) * (this->z - z));
}

void Vetor::setCoordenadas(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

void Vetor::sum(Vetor v)
{
    setCoordenadas(this->x + v.x, this->y + v.y, this->z + v.z);
}

void Vetor::sub(Vetor v)
{
    setCoordenadas(this->x - v.x, this->y - v.y, this->z - v.z);
}

void Vetor::mult(double m)
{
    setCoordenadas(this->x * m, this->y * m, this->z * m);
}

void Vetor::sub(double x, double y, double z)
{
    setCoordenadas(this->x - x, this->y - y, this->z - z);
}