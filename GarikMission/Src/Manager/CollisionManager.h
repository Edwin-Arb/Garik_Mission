#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../GameMap/GameMap.h"

class ACollisionManager
{
public:
    ACollisionManager(const AEnemy& Enemy, const APlayer& Player, const AGameMap& GameMap);

    ~ACollisionManager() = default;
    
    void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr) const;

private:
    bool CheckPositionBulletWithScreen(const ABullet& Bullet) const;
    bool CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& EnemyRect) const;

private:
    const AEnemy& EnemeRef;
    const APlayer& PlayerRef;
    const AGameMap& GameMapRef;
};
