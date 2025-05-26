#ifndef ROCKET_HPP
#define ROCKET_HPP
#include "Turret.hpp"

class Rocket : public Turret {
public:
    static const int Price;
    Rocket(float x, float y);
    void CreateBullet() override;
    int shotsCount = 0;
};

#endif