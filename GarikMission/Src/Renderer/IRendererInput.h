#pragma once
#include "../Enemy/Enemy.h"
#include "../Bullet/Bullet.h"
#include "../Renderer/IRendererSprite.h"


class IRendererInput
{
public:
    IRendererInput &operator= (const IRendererInput&) = delete;
    IRendererInput &operator=(IRendererInput&&) = delete;
    
    virtual ~IRendererInput() = default;
    
    virtual void PlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, const sf::FloatRect& PlayerRect, IRendererSprite& Renderer) = 0;
    virtual void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPrt, const AEnemy& EnemyCollision, float DeltaTime) = 0;
};
