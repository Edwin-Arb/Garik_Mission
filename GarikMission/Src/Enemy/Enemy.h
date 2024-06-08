#pragma once
#include "../Bullet/Bullet.h"
#include "../Player/Player.h"
#include "../GameMap/GameMap.h"
#include "../UserInterface/HealthBar.h"


class AEnemy
{
public:
    AEnemy(const int Health, const sf::Vector2f& StartPosition);
    ~AEnemy();

    void InitEnemy(ASpriteManager& SpriteManager);
    void CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
                                    const sf::Vector2f& EnemySize,
                                    const sf::Vector2f& DrawScale);
    void DetectPlayer(const APlayer& Player, const AGameMap& GameMap);
    void UpdateDirectionAndVelocity(float DeltaTime, const APlayer& Player);
    void UpdatePosition();
    void UpdateDetectionAreaPosition();
    void UpdateMoveDistance();
    void UpdateEnemyMove(float DeltaTime, const ::APlayer& Player, const AGameMap& GameMap);

    void SetEnemyHealth(int Damage);
    int GetEnemyHealth() const;
    sf::FloatRect GetEnemyRect() const;

    void DrawEnemy(sf::RenderWindow& Window);

private:
    // Переменные для хранения текущего направления и максимальной дистанции
    bool bMovingRight;
    bool bIsPlayerDetected;
    int EnemyHealth;
    float MinMoveDistance;
    float MaxMoveDistance;

    sf::Vector2f EnemyStartPosition;
    sf::Vector2f EnemyVelocity;
    sf::FloatRect EnemyRect;

    // Переменная для хранения позиции отрисовки врага
    sf::Vector2f EnemyDrawPosition;

    sf::Texture* EnemyTexturePtr;
    sf::Sprite EnemySprite;

    // TODO Нужен для тестирования обнаружения персонажа, удалить после тестов
    sf::RectangleShape LineTraceDetectionArea;

    AHealthBar *EnemyHealthBarPtr;
};
