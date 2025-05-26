#include <allegro5/base.h>
#include <cmath>
#include <string>
#include "Rocket.hpp"
#include "Bullet/RocketBullet.hpp"
#include "Engine/AudioHelper.hpp"
#include "Engine/Group.hpp"
#include "Engine/Point.hpp"
#include "Scene/PlayScene.hpp"
#include "Enemy/TankEnemy.hpp"

const int Rocket::Price = 250;

Rocket::Rocket(float x, float y)
    : Turret("play/tower-base.png", "play/turret-3.png", x, y, 250, Price, 1.5) {
    // WIP
}

void Rocket::CreateBullet() {
    if(shotsCount >= 2){
        Enabled = false;
        return;
    }
    PlayScene* scene = getPlayScene();
    Enemy* targetTank = nullptr;
    // find a TankEnemy in range
    for(auto& obj : scene->EnemyGroup->GetObjects()){
        Enemy* enemy = dynamic_cast<Enemy*>(obj);
        if(dynamic_cast<TankEnemy*>(enemy)){
            Engine::Point diff = enemy->Position - Position;
            if(diff.Magnitude() <= CollisionRadius){
                targetTank = enemy;
                break;
            }
        }
    }
    if(targetTank){
        Engine::Point diff = targetTank->Position - Position;
        float rotation = atan2(diff.y, diff.x);
        Engine::Point normalized = diff.Normalize();
        scene->BulletGroup->AddNewObject(new RocketBullet(Position, normalized, rotation, this));
        ++shotsCount;
        if(shotsCount >= 2){
            Enabled = false;
        }
    }
}