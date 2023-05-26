#pragma once

class Vetor
{
public:
    double x, y, z;

    Vetor(double x = 0, double y = 0, double z = 0)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void setCoordenadas(double x, double y, double z);

    double dist(Vetor v);
    double dist(double x = 0, double y = 0, double z = 0);
    void sum(Vetor v);
    void sub(Vetor v);
};