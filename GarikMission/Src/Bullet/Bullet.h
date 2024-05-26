#pragma once
#include "../Manager/SpriteManager.h"


class ABullet
{
public:
    ABullet(const sf::Vector2f& StartPosition, ASpriteManager& RendererSprite);
    ~ABullet();

    //bool DoShapesCollide(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2);
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

