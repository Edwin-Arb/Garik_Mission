#pragma once
#include "../Manager/SpriteManager.h"


class ABullet
{
public:
    ABullet(const bool NewVelocity, const sf::Vector2f& StartPosition, ASpriteManager& RendererSprite);
    ~ABullet();
    
    bool IsCollided() const;
    void UpdateBulletPosition(float DeltaTime);
    void HandleCollision(const sf::FloatRect& Obstacle);
    sf::FloatRect GetBulletCollider() const;
    
    void DrawBullet(sf::RenderWindow& Window);

private:
    bool bIsCollided = false;
    
    // Направление и коллизия
    sf::Vector2f BulletVelocity;
    sf::FloatRect BulletRect;

    // Текстура пули
    sf::Texture *BulletTexturePtr;
    sf::Sprite BulletSprite;
    
};

