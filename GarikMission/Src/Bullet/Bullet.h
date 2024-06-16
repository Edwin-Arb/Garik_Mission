#pragma once
#include "../Abstract/AActor.h"
#include "../Manager/SpriteManager.h"

/**
 * @brief Перечисление, определяющее различные типы пуль.
 */
enum class EBulletType
{
    EBT_ShootAtPlayer, // Пуля направлена на игрока.
    EBT_ShootAtEnemy   // Пуля направлена на врага.
};

/**
 * @brief Класс ABullet представляет собой сущность пули в игре.
 * Пуля может двигаться с заданной скоростью, проверять коллизии
 * с другими объектами и отрисовываться на экране.
 */
class ABullet : public AActor
{
public:
    /**
     * @brief Конструктор для инициализации пули.
     * 
     * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
     * @param ShootAt Тип цели, на которую направлена пуля.
     * @param StartPosition Начальная позиция пули.
     * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
     */
    ABullet(const bool NewVelocity, const EBulletType& ShootAt,
            const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABullet() override;

    /**
     * @brief Инициализирует текструру и создаёт спрайт для пули.
     * 
     * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
     * @param StartPosition Начальная позиция пули.
     * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
     */
    void InitBullet(bool NewVelocity, const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager);

    /**
     * @brief Обновляет позицию пули на основе прошедшего времени.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     */
    void UpdateBulletPosition(float DeltaTime);

    /**
     * @brief Возвращает урон, наносимый пулей.
     * 
     * @return Значение урона пули.
     */
    int GetBulletDamage() const;

    /**
     * @brief Возвращает тип пули.
     * 
     * @return Тип пули (EBulletType).
     */
    EBulletType GetBulletType() const;

    /**
     * @brief Отрисовывает пулю на указанном окне отрисовки.
     * 
     * @param Window Окно отрисовки, на котором отображается пуля.
     */
    void DrawActor(sf::RenderWindow& Window) override;

private:
    int BulletDamage;                 // Урон, наносимый пулей.
    EBulletType BulletType;           // Тип пули (игрок или враг).
    ASpriteManager* SpriteManagerPtr; // Указатель на менеджер спрайтов.
};
