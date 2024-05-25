#pragma once
#include "../Renderer/IRendererInput.h"


class AInputManager : public IRendererInput
{
public:
    void PlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, const sf::FloatRect& PlayerRect, IRendererSprite& Renderer) override;
    void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPrt, const AEnemy& EnemyCollision, float DeltaTime) override;
};
