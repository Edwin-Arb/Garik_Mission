﻿#pragma once

#include "../Enemy/Enemy.h"
#include "../Enemy/BossEnemy/BossEnemy.h"
#include "../GameObjects/KeyObject.h"
#include "../GameMap/GameMap.h"
#include "../GameObjects/ChestObject.h"
#include "../Manager/ParticleSystemManager.h"

// TODO: форвард декларейшен
class AKeyObject;

/**
 * @brief Менеджер коллизий для персонажа и врагов.
 * 
 * Класс ACollisionManager отвечает за проверку коллизий персонажа с игровой картой и пулями,
 * а также врагов с игровой картой.
 */
class ACollisionManager
{
public:
    /**
     * @brief Конструктор класса ACollisionManager.
     * 
     * Инициализирует ссылки на персонажа и игровую карту.
     * 
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     */
    ACollisionManager(APlayer& Player, AGameMap& GameMap);

    /**
     * @brief Деструктор класса ACollisionManager.
     */
    ~ACollisionManager() = default;

    /**
    * @brief Проверяет пересечение двух прямоугольников.
    * 
    * @param Rect1 Первый прямоугольник.
    * @param Rect2 Второй прямоугольник.
    * @return true, если прямоугольники пересекаются, иначе false.
    */
    bool CheckCollision(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2);

private:
    /**
     * @brief Проверяет столкновение пули с игровой картой.
     * 
     * @param Bullet Пуля для проверки столкновения.
     * @return true, если есть столкновение с игровой картой, иначе false.
     */
    bool CheckBulletCollisionWithGameMap(AActor& Bullet) const;

    /**
     * @brief Проверяет столкновение пули с объектом (персонажем или врагом).
     * 
     * @param Bullet Пуля для проверки столкновения.
     * @param PawnRect Прямоугольник объекта для проверки.
     * @return true, если есть столкновение с объектом, иначе false.
     */
    bool CheckBulletCollisionWithPawn(AActor& Bullet, const sf::FloatRect& PawnRect) const;

public:
    /**
     * @brief Проверяет столкновения всех пуль с врагами и игровой картой.
     * 
     * @param BulletsVectorPtr Указатель на вектор пуль.
     * @param EnemyVectorPtr Указатель на вектор врагов.
     * @param KeysVectorPtr
     * @param Player Ссылка на персонажа.
     * @param SpriteManagerPtr
     * @param ParticleSystem Ссылка на менеджер частиц. // TODO: Тест анимация частиц.
     */
    void CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr,
                                  std::vector<AEnemy*>& EnemyVectorPtr,
                                  std::vector<AKeyObject*>& KeysVectorPtr,
                                  APlayer& Player, ASpriteManager& SpriteManagerPtr,
                                  AParticleSystemManager& ParticleSystem) const;

    /**
   * @brief Обрабатывает столкновение пешки с игровой картой.
   * 
   * @param PawnRect Прямоугольник коллизии персонажа.
   * @param ObjectVelocity Вектор скорости объекта персонажа.
   * @param EnemyPtr Указатель на врага.
   * @param bCanJump Может ли персонаж прыгать.
   * @param bCanClimb Может ли персонаж карабкаться.
   */
    void HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect,
                                        sf::Vector2f& ObjectVelocity,
                                        bool& bCanJump,
                                        bool& bCanClimb,
                                        AEnemy* EnemyPtr = nullptr) const;

    // TODO: добавить документацию
    void CheckCollisionWithKey(APlayer& Player, std::vector<AKeyObject*>& KeysVectorPtr);

    // TODO: добавить документацию
    //void CheckCollisionWithChest(APlayer& Player, std::vector<AChestObject*>& ChestVectorPtr);

    // TODO: добавить документацию
    void CheckCollisionWithGameMap(sf::FloatRect& ActorRect, sf::Vector2f& ObjectVelocity) const;

private:
    APlayer& PlayerRef; // Ссылка на объект персонажа.
    AGameMap& GameMapRef; // Ссылка на объект игровой карты.
};
