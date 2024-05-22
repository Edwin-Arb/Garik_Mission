#pragma once
#include "Constants.h"
#include "Bullet.h"
#include "Enemy.h"

class APlayer
{
public:
    APlayer();
    
    void InitPlayer();
    void UpdatePlayerMove(const sf::Event& event, float delta_time);
    sf::FloatRect GetPlayerRect() const;
    void DrawPlayer(sf::RenderWindow& window);

private:
    // Параметры персонажа
    bool isPlayerOnGround;
    float playerSpeed;
    float playerJumpSpeed;

    // Направление и коллизия
    sf::Vector2f playerVelocity;
    sf::FloatRect playerRect;

    // Текстура персонажа
    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    // Таймер, для задержки между выстрелами
    sf::Clock delayShootTime;

    // Добавляем в массив пули, которые выстрелили, чтобы их отрисовать
    std::vector<ABullet> bullets;
    
    // для тестов коллизии, потом удалить
    sf::RectangleShape playerShapeCollision;

    // Test
    AEnemy enemy;
};
