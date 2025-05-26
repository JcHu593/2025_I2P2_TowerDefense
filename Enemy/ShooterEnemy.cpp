#include "ShooterEnemy.hpp"
#include "Scene/PlayScene.hpp"
#include "Engine/Point.hpp"
#include "Turret/Turret.hpp"
#include "Bullet/FireBullet.hpp"

ShooterEnemy::ShooterEnemy(int x, int y)
    : Enemy("play/enemy-6.png", x, y, 10, 35, 40, 12) {
}

void ShooterEnemy::Update(float deltaTime) {
    Enemy::Update(deltaTime);
    shootTimer += deltaTime;
    if (shootTimer >= shootCooldown) {
        shootTimer = 0;
        PlayScene* scene = getPlayScene();
        for(auto& obj : scene->TowerGroup->GetObjects()){
            Turret* turret = dynamic_cast<Turret*>(obj);
            if(turret){
                float dist = (turret->Position - Position).Magnitude();
                if(dist <= shootRange){
                    Engine::Point diff = turret->Position - Position;
                    float rotation = atan2(diff.y, diff.x);
                    Engine::Point normalized = diff.Normalize();
                    scene->BulletGroup->AddNewObject(
                        new FireBullet(Position + normalized * 36, diff, rotation, nullptr)
                    );
                    turret->Hit(shootDamage);
                    break;
                }
            }
        }
    }
}