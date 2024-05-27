#include "Bullet.h"


ABullet::ABullet(const sf::Vector2f& StartPosition, ASpriteManager& RendererSprite)
    : BulletTexturePtr(new sf::Texture)
{
    // Установить направление, откуда начать движение
    BulletVelocity = {BULLET_SPEED, 0.f}; // Скорость и направление пули

    // Устанавливаем размер коллизии для пули
    BulletRect = {StartPosition.x, StartPosition.y, 3.f, 3.f};

    // Получить файл текстуры
    assert(BulletTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/Bullets.png"));

    // Установить спрайт для пули
    BulletSprite.setTexture(*BulletTexturePtr);
    BulletSprite.setTextureRect(sf::IntRect(7, 72, 3, 3));

    // Установить масштаб пули
    RendererSprite.SetSpriteSize(BulletSprite, BULLET_SIZE.x * (DRAW_SCALE.x - 3),
                                                 BULLET_SIZE.y * (DRAW_SCALE.y - 3));

    // Установить центр спрайта
    RendererSprite.SetSpriteRelativeOrigin(BulletSprite, 0.5f, 0.5f);

    // Задать стартовую позицию, т.е откуда начинает стрелять оружие
    BulletSprite.setPosition(StartPosition.x, StartPosition.y);
}

ABullet::~ABullet()
{
    delete BulletTexturePtr;
}

void ABullet::UpdateBulletPosition(float DeltaTime)
{
    BulletVelocity += BulletVelocity * DeltaTime;

    // Обновляем положение спрайта пули
    BulletSprite.move(BulletVelocity * DeltaTime);

    // Коллизия верхнего левого угла
    BulletRect.left = BulletSprite.getPosition().x;
    BulletRect.top = BulletSprite.getPosition().y;

    BulletRect.width = BulletRect.left + BULLET_SIZE.x;
    BulletRect.height = BulletRect.top + BULLET_SIZE.y;
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
