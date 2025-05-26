#include "RocketBullet.hpp"
#include "Enemy/TankEnemy.hpp"
#include "Engine/Group.hpp"
#include "Scene/PlayScene.hpp"

RocketBullet::RocketBullet(Engine::Point position, Engine::Point forwardDirection, float rotation, Turret* parent)
    : Bullet("play/bullet-4.png", 200, 100, position, forwardDirection, rotation, parent) {
}

void RocketBullet::OnExplode(Enemy* enemy){
    if(dynamic_cast<TankEnemy*>(enemy)){
        // enemy->Hit(100);
    }
    else {
        // enemy->Hit(0);
    }
}