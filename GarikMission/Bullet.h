#pragma once
#include "Constants.h"

class ABullet
{
public:
    ABullet();
    ~ABullet();

    ABullet(float deltaTime);
    void SetPosition(float posX, float posY);
    void SetVelocity(float deltaTime);
    void DrawBullet(sf::RenderWindow& window);

private:
    sf::Vector2f bulletVelocity;
    sf::FloatRect bulletRect;

    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
};
