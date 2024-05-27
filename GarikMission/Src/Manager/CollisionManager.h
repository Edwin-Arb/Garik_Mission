#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"

class ACollisionManager
{
public:
    ACollisionManager(AEnemy &Enemy);
    
    ~ACollisionManager() = default;
    
    bool CheckPositionBulletWithScreen(const ABullet& Bullet) const;
    bool CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& EnemyRect) const;
    void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr);

private:
    AEnemy &EnemeRef;
};
