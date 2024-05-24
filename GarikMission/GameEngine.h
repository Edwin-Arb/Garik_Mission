#pragma once
#include "Constants.h"
#include "GameState.h"


class AGameEngine
{
public:
       
    static void PlayerShoot(std::vector<ABullet*>& BulletsVectorPtr, const sf::FloatRect& PlayerRect);
    static void CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPrt, const AEnemy& EnemyCollision, float DeltaTime);
    
    static void SetSpriteSize(sf::Sprite& Sprite, float DesiredWidth, float DesiredHeight);
    static void SetShapeSize(sf::Shape& Shape, float DesiredWidth, float DesiredHeight);

    static void SetSpriteRelativeOrigin(sf::Sprite& Sprite, float OriginX, float OriginY);
    static void SetShapeRelativeOrigin(sf::Shape& Shape, float OriginX, float OriginY);
    //void DrawGame(sf::RenderWindow& window);
    
};
