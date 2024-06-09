#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../GameMap/GameMap.h"

class ACollisionManager
{
public:
    ACollisionManager(APlayer& Player, AGameMap& GameMap);

    ~ACollisionManager() = default;

private:
    bool CheckBulletCollisionWithGameMap(const ABullet& Bullet) const;
    bool CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& EnemyRect) const;
    bool CheckBulletCollisionWithPlayer(const ABullet& Bullet, const sf::FloatRect& PlayerRect) const;

public:
    void CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr, std::vector<AEnemy*>& EnemysVectorPtr) const;
    void HandlePlayerCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity, bool& bCanJump) const;

private:
    APlayer& PlayerRef;
    AGameMap& GameMapRef;
};
