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
    
    // Инициализация врага
    void InitEnemy(ASpriteManager& SpriteManager);

    void EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const;

    // Вычисление позиции отрисовки врага
    void CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
                                    const sf::Vector2f& EnemySize,
                                    const sf::Vector2f& DrawScale);

    // Обнаружение игрока
    void DetectPlayer(const APlayer& Player, const AGameMap& GameMap);

    // Обновление направления и скорости врага
    void UpdateDirectionAndVelocity(float DeltaTime, const APlayer& Player);

    // Обновление позиции врага
    void UpdatePosition();

    // Обновление позиции зоны обнаружения игрока
    void UpdateDetectionAreaPosition();

    // Обновление дистанции движения врага
    void UpdateMoveDistance();

    // Обновление движения врага
    void UpdateEnemyMove(float DeltaTime, const APlayer& Player, const AGameMap& GameMap);

    // Установка здоровья врага
    void SetEnemyHealth(int Damage);

    // Получение здоровья врага
    int GetEnemyHealth() const;

    // Получение прямоугольника врага
    sf::FloatRect GetEnemyRect() const;

    // Отрисовка врага
    void DrawEnemy(sf::RenderWindow& Window);

private:
    // Переменные для хранения текущего направления и максимальной дистанции
    bool bIsMoveRight;
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
