#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../GameMap/GameMap.h"

class ACollisionManager
{
public:
    ACollisionManager(AEnemy& Enemy, APlayer& Player, AGameMap& GameMap);

    ~ACollisionManager() = default;

private:
    bool CheckBulletCollisionWithGameMap(const ABullet& Bullet) const;
    bool CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& EnemyRect) const;

public:
    void CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr) const;
    void HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity) const;
    void HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity, bool& bCanJump) const;

private:
    AEnemy& EnemeRef;
    APlayer& PlayerRef;
    AGameMap& GameMapRef;
};
