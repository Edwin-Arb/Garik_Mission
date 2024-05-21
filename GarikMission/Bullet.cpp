#include "Bullet.h"

ABullet::ABullet(float deltaTime)
    :
      bulletRect({3.f, 3.f, 3.f, 3.f})
{
    bulletVelocity.x = SPEED_PAWN * deltaTime;
    bulletVelocity.y = 100.f;
    bulletTexture.loadFromFile(RESOURCES_PATH + "MainTiles/bullets.png");
    bulletSprite.setTexture(bulletTexture);

    bulletSprite.setTextureRect(sf::IntRect(7, 72,
                                3.f,
                                3.f));
    bulletSprite.setScale(DRAW_SCALE.x, DRAW_SCALE.y);
}

// ABullet::~ABullet()
// {
//     delete this;
// }

void ABullet::SetPosition(float posX, float posY)
{
    bulletSprite.setPosition(posX, posY);
}

void ABullet::SetVelocity(float deltaTime)
{
    bulletVelocity.x = SPEED_PAWN * deltaTime;
}

void ABullet::DrawBullet(sf::RenderWindow &window)
{
    bulletSprite.setPosition(bulletVelocity.x, bulletVelocity.y);
    window.draw(bulletSprite);
}
