#pragma once

#include "../Enemy.h"

/**
 * @brief Класс ABossEnemy представляет собой босса в игре.
 */
class ABossEnemy : public AEnemy
{
public:
    /**
     * @brief Конструктор для инициализации босса.
     * 
     * @param MaxPatrolDistance Максимальная дистанция патрулирования босса.
     * @param StartPosition Начальная позиция босса.
     */
    ABossEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABossEnemy() override = default;

    /**
     * @brief Инициализирует босса текстурами и анимациями.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
     */
    void InitEnemy(ASpriteManager& SpriteManager) override;

    /**
     * @brief Выполняет выстрелы босса.
     * 
     * @param SpawnBulletOffsetY Смещение по оси Y для спавна пули.
     * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
     * @param SpriteManager Менеджер спрайтов для управления спрайтами.
     */
    void EnemyShoot(const float SpawnBulletOffsetY, std::vector<ABullet*>& BulletsVectorPtr,
                    ASpriteManager& SpriteManager) override;

    /**
     * @brief Обновляет движение босса.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     * @param Player Ссылка на объект игрока.
     * @param GameMap Ссылка на объект игровой карты.
     * @param CollisionManager Менеджер коллизий.
     */
    void UpdateEnemyMove(float DeltaTime, APlayer& Player, const AGameMap& GameMap,
                         const ACollisionManager& CollisionManager) override;

private:
    /**
     * @brief Выполняет специальную атаку босса.
     * 
     * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
     * @param SpriteManager Менеджер спрайтов для управления спрайтами.
     */
    void SpecialAttack(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const;

    float BossSpecialAttackCooldown; // Время перезарядки специальной атаки босса.
    float CurrentSpecialAttackCooldown; // Текущее время до следующей специальной атаки.

    // TODO: Test collision
    sf::RectangleShape RectCollisionTest;
};
