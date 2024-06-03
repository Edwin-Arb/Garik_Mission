#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../GameMap/GameMap.h"

class ACollisionManager
{
public:
    ACollisionManager(AEnemy& Enemy, APlayer& Player, AGameMap& GameMap);

    ~ACollisionManager() = default;
    
    void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr) const;

private:
    bool CheckPositionBulletWithScreen(const ABullet& Bullet) const;
    bool CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& EnemyRect) const;

private:
    AEnemy& EnemeRef;
    APlayer& PlayerRef;
    AGameMap& GameMapRef;
};
