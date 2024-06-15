#pragma once
#include "../Player/Player.h"
#include "../Enemy/Enemy.h"
#include "../GameMap/GameMap.h"

/**
 * @brief Менеджер коллизий для персонажа и врагов.
 * 
 * Управляет проверкой коллизий персонажа с игровой картой и пулями, а также врагов с игровой картой.
 */
class ACollisionManager
{
public:
    /**
     * @brief Конструктор класса ACollisionManager.
     * 
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     */
    ACollisionManager(APlayer& Player, AGameMap& GameMap);

    /**
     * @brief Деструктор класса ACollisionManager.
     */
    ~ACollisionManager() = default;

private:
    /**
     * @brief Проверяет столкновение пули с игровой картой.
     * 
     * @param Bullet Пуля для проверки.
     * 
     * @return true, если есть столкновение с игровой картой, иначе false.
     */
    bool CheckBulletCollisionWithGameMap(const ABullet& Bullet) const;

    /**
     * @brief Проверяет столкновение пули с объектом.
     * 
     * @param Bullet Пуля для проверки.
     * @param PawnRect Прямоугольник объекта для проверки.
     * 
     * @return true, если есть столкновение с объектом, иначе false.
     */
    bool CheckBulletCollisionWithPawn(const ABullet& Bullet, const sf::FloatRect& PawnRect) const;

public:
    /**
     * @brief Проверяет столкновения всех пуль с врагами и игровой картой.
     * 
     * @param BulletsVectorPtr Указатель на вектор пуль.
     * @param EnemysVectorPtr Указатель на вектор врагов.
     */
    void CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr, std::vector<AEnemy*>& EnemysVectorPtr, APlayer &PlayerPtr) const;

    /**
     * @brief Обрабатывает столкновение персонажа с игровой картой.
     * 
     * @param PawnRect Прямоугольник персонажа для проверки и обработки столкновений.
     * @param ObjectVelocity Скорость объекта (персонажа).
     * @param bCanJump Флаг, позволяющий персонажу прыгать.
     * @param bCanClimb Флаг, позволяющий персонажу взбираться.
     */
    void HandlePlayerCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity, bool& bCanJump,
                                          bool& bCanClimb) const;

private:
    APlayer& PlayerRef;   // Ссылка на персонажа.
    AGameMap& GameMapRef; // Ссылка на игровую карту.
};
