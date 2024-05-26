#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

class ACollisionManager
{
public:
    bool CheckPositionBulletWithScreen(const ABullet& Bullet) const;
    bool CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& Enemy) const;
    void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr);
};
