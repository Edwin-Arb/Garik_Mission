#pragma once

#include "Enemy.h"

class ABossEnemy : public AEnemy
{
public:
    ABossEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);
    ~ABossEnemy() override = default;

    void InitEnemy(ASpriteManager& SpriteManager) override;
    void EnemyShoot(const float SpawnBulletOffsetY, std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) override;
    //void CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
    //                                const sf::Vector2f& EnemySize,
    //                                const sf::Vector2f& DrawScale) override;
    void UpdateEnemyMove(float DeltaTime, APlayer& Player, const AGameMap& GameMap,
                         const ACollisionManager& CollisionManager) override;

private:
    void SpecialAttack(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const;

public:
    void DrawActor(sf::RenderWindow& Window) override;

private:
    float BossSpecialAttackCooldown;
    float CurrentSpecialAttackCooldown;

    // TODO: Test collision
    sf::RectangleShape RectCollisionTest;

    sf::Texture BossTexture;
    sf::Sprite BossSprite;
};
