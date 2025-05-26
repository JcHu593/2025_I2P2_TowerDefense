#ifndef SHOOTERENEMY_HPP
#define SHOOTERENEMY_HPP
#include "Enemy.hpp"

class ShooterEnemy : public Enemy {
public:
    float shootCooldown = 0.5f;
    float shootTimer = 0.0f;
    float shootRange = 120.0f;
    float shootDamage = 20.0f;
    ShooterEnemy(int x, int y);
    void Update(float deltaTime) override;
};

#endif