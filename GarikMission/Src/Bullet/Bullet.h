#pragma once
#include "../Manager/SpriteManager.h"


class ABullet
{
public:
    ABullet(const bool NewVelocity, const sf::Vector2f& StartPosition, ASpriteManager& RendererSprite);
    ~ABullet();
    
    void UpdateBulletPosition(float DeltaTime);
    int GetBulletDamage() const;
    sf::FloatRect GetBulletCollider() const;
    
    void DrawBullet(sf::RenderWindow& Window);

private:
    // Урон от пули
    int BulletDamage;
    
    // Направление и коллизия
    sf::Vector2f BulletVelocity;
    sf::FloatRect BulletRect;

    // Текстура пули
    sf::Texture *BulletTexturePtr;
    sf::Sprite BulletSprite;
    
};

