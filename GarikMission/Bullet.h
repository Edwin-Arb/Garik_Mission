#pragma once
#include "Constants.h"


class ABullet
{
public:
    ABullet(const sf::Vector2f& start_position);

    bool CheckPositionBulletWithScreen() const;
    bool CheckBulletCollisionWithEnemy(const sf::FloatRect& enemy) const;
    void UpdateBulletPosition(float deltaTime);
    
    sf::FloatRect GetBulletCollider() const;
    
    void DrawBullet(sf::RenderWindow& window);

private:
    // Направление и коллизия
    sf::Vector2f bulletVelocity;
    sf::FloatRect bulletRect;

    // Текстура пули
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
};

