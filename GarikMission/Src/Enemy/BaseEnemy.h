#pragma once
#include "Enemy.h"

class ABaseEnemy : public AEnemy
{
public:
    ABaseEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);
    ~ABaseEnemy() override = default;
    
    void InitEnemy(ASpriteManager& SpriteManager) override;
    //void EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) override;
    //void UpdateEnemyMove(float DeltaTime, APlayer& Player, const AGameMap& GameMap, const ACollisionManager& CollisionManager) override;

private:
    
};
