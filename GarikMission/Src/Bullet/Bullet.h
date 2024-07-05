#pragma once
#include "../Abstract/AActor.h"
#include "../Manager/SpriteManager.h"
#include "../Manager/AnimationManager.h"

/**
 * @brief Перечисление, определяющее различные типы пуль.
 */
enum class EBulletType
{
    EBT_ShootAtPlayer, // Пуля направлена на игрока.
    EBT_ShootAtEnemy // Пуля направлена на врага.
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
     * @param BulletTextureRect Прямоугольник текстуры пули.
     * @param ShootAt Тип цели, на которую направлена пуля.
     * @param StartPosition Начальная позиция пули.
     * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
     */
    ABullet(const bool NewVelocity, const sf::IntRect& BulletTextureRect, const EBulletType& ShootAt,
            const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager);

    /**
     * @brief Деструктор для освобождения ресурсов.
     */
    ~ABullet() override = default;

    /**
     * @brief Инициализирует текстуру и создаёт спрайт для пули.
     * 
     * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
     * @param BulletTextureRect Прямоугольник текстуры пули.
     * @param StartPosition Начальная позиция пули.
     * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
     */
    virtual void InitBullet(const bool NewVelocity, const sf::IntRect& BulletTextureRect,
                            const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager);

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
    int BulletDamage; // Урон, наносимый пулей.
    EBulletType BulletType; // Тип пули (игрок или враг).

    // TODO: Для тестирования разрушения анимации пули
    bool bIsDestroyed; // Флаг, показывающий, уничтожена ли пуля.
    AAnimationManager AnimBulletDestroy; // Анимация уничтожения пули.
};
