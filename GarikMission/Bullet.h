#pragma once
#include "Constants.h"


class ABullet
{
public:
    ABullet(const sf::Vector2f& StartPosition);
    ~ABullet();

    //bool DoShapesCollide(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2);
    bool CheckPositionBulletWithScreen() const;
    bool CheckBulletCollisionWithEnemy(const sf::FloatRect& Enemy) const;
    void UpdateBulletPosition(float DeltaTime);
    
    sf::FloatRect GetBulletCollider() const;
    
    void DrawBullet(sf::RenderWindow& Window);

private:
    // Направление и коллизия
    sf::Vector2f BulletVelocity;
    sf::FloatRect BulletRect;

    // Текстура пули
    sf::Texture *BulletTexturePtr;
    sf::Sprite BulletSprite;
};

