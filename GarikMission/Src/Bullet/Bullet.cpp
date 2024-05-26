﻿#include "Bullet.h"


ABullet::ABullet(const sf::Vector2f& StartPosition, ASpriteManager &RendererSprite)
    : BulletTexturePtr(new sf::Texture)
{
    // Установить направление, откуда начать движение
    BulletVelocity = {BULLET_SPEED, 0.f}; // Скорость и направление пули

    // Устанавливаем размер коллизии для пули
    BulletRect = {StartPosition.x, StartPosition.y, 3.f, 3.f};

    // Получить файл текстуры
    assert(BulletTexturePtr->loadFromFile(RESOURCES_PATH + "MainTiles/Bullets.png"));

    // Установить спрайт для пули
    BulletSprite.setTexture(*BulletTexturePtr);
    BulletSprite.setTextureRect(sf::IntRect(7, 72, 3, 3));
    
    // Установить масштаб пули
    RendererSprite.SetSpriteSize(BulletSprite, BULLET_SIZE.x * DRAW_SCALE.x, BULLET_SIZE.y * DRAW_SCALE.y);

    // Установить центр спрайта
    RendererSprite.SetSpriteRelativeOrigin(BulletSprite, 0.5f, 0.5f);
    
    // Задать стартовую позицию, т.е откуда начинает стрелять оружие
    BulletSprite.setPosition(StartPosition.x, StartPosition.y);
}

ABullet::~ABullet()
{
    delete BulletTexturePtr;
}

// bool ABullet::DoShapesCollide(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2)
// {
//     return (Rect1.left < Rect2.left + Rect2.width) &&
//         (Rect1.left + Rect1.width > Rect2.left) &&
//         (Rect1.top < Rect2.top + Rect2.height) &&
//         (Rect1.top + Rect1.height > Rect2.top);
// }

void ABullet::UpdateBulletPosition(float DeltaTime)
{
    BulletVelocity += BulletVelocity * DeltaTime;

    // Обновляем положение спрайта пули
    BulletSprite.move(BulletVelocity * DeltaTime);

    // Коллизия верхнего левого угла
    BulletRect.left = BulletSprite.getPosition().x;
    BulletRect.top = BulletSprite.getPosition().y;

    BulletRect.width = BulletRect.left + 3.f;
    BulletRect.height = BulletRect.top + 3.f;
}



sf::FloatRect ABullet::GetBulletCollider() const
{
    // Получить коллизию пули
    return BulletRect;
}

void ABullet::DrawBullet(sf::RenderWindow& Window)
{    
    // Установить положение спрайта с коллизией, и отрисовать на экран
    BulletSprite.setPosition(BulletRect.left, BulletRect.top);
    Window.draw(BulletSprite);
}
