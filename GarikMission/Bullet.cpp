#include "Bullet.h"

ABullet::ABullet(const sf::Vector2f& start_position)
{
    // Установить направление, откуда начать движение
    bulletVelocity = {BULLET_SPEED, 0.f}; // Скорость и направление пули

    // Устанавливаем размер коллизии для пули
    bulletRect = {start_position.x, start_position.y, 3.f, 3.f};

    // Получить файл текстуры
    assert(bulletTexture.loadFromFile(RESOURCES_PATH + "MainTiles/bullets.png"));

    // Установить спрайт для пули
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setTextureRect(sf::IntRect(7, 72, 3.f, 3.f));
    bulletSprite.setScale(BULLET_SCALE);
    bulletSprite.setPosition(start_position.x, start_position.y);
}

void ABullet::UpdateBulletPosition(float deltaTime)
{
    bulletVelocity += bulletVelocity * deltaTime;

    // Обновляем положение спрайта пули
    bulletSprite.move(bulletVelocity * deltaTime);

    // Коллизия верхнего левого угла
    bulletRect.left = bulletSprite.getPosition().x;
    bulletRect.top = bulletSprite.getPosition().y;

    bulletRect.width = bulletRect.left + 3.f;
    bulletRect.height = bulletRect.top + 3.f;
}

bool ABullet::CheckPositionBulletWithScreen() const
{// Если дистанция пули выходит за границы экрана, то возвращаем true
    return (bulletRect.left <= 100.f) || (bulletRect.width >= SCREEN_WIDTH - 100.f);
}

bool ABullet::CheckBulletCollisionWithEnemy(const sf::FloatRect &enemy) const
{// Проверка столкновения с врагом
    return bulletRect.intersects(enemy);
}

sf::FloatRect ABullet::GetBulletCollider() const
{// Получить коллизию пули
    return bulletRect;
}

void ABullet::DrawBullet(sf::RenderWindow& window)
{// Установить положение спрайта с коллизией, и отрисовать на экран
    bulletSprite.setPosition(bulletRect.left, bulletRect.top);
    window.draw(bulletSprite);
}
