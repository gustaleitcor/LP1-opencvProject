#include "Player.h"

Player::Player(double x, double y, double z)
{
    pos.setCoordenadas(x, y, z);
    vel.setCoordenadas(0, 0, 0);
    acc.setCoordenadas(0, 0, 0);
}

Player::Player()
{
    pos.setCoordenadas(0, 0, 0);
    vel.setCoordenadas(0, 0, 0);
    acc.setCoordenadas(0, 0, 0);
}

void Player::atualizar()
{
    vel.sum(acc);
    pos.sum(vel);
}