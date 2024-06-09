#pragma once
#include "../Manager/SpriteManager.h"

enum class EBulletType
{
    EBT_ShootAtPlayer,  // Пуля летит в игрока
    EBT_ShootAtEnemy    // Пуля летит во врага
};

class ABullet
{
public:
    ABullet(const bool NewVelocity, const EBulletType &ShootAt,  const sf::Vector2f& StartPosition, ASpriteManager& RendererSprite);
    ~ABullet();

    // Проверка попадания в цель
    bool CheckCollision(const sf::FloatRect& TargetRect) const;
    
    // Обновление позиции пули
    void UpdateBulletPosition(float DeltaTime);
    
    // Получение урон от пули
    int GetBulletDamage() const;

    // Получение прямоугольника пули
    sf::FloatRect GetBulletCollider() const;

    // Получение типа пули
    EBulletType GetBulletType() const;
    
    // Отрисовка пули
    void DrawBullet(sf::RenderWindow& Window);

private:
    // Урон от пули
    int BulletDamage;

    EBulletType BulletType;
    
    // Направление и коллизия
    sf::Vector2f BulletVelocity;
    sf::FloatRect BulletRect;

    // Текстура пули
    sf::Texture *BulletTexturePtr;
    sf::Sprite BulletSprite;
    
};

