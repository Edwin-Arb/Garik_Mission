#pragma once
#include "../Manager/SpriteManager.h"

/**
 * Перечисление, определяющее различные типы пуль.
 */
enum class EBulletType
{
    EBT_ShootAtPlayer, // Пуля направлена на игрока.
    EBT_ShootAtEnemy   // Пуля направлена на врага.
};

/**
 * Класс ABullet представляет собой сущность пули в игре.
 * Пуля может двигаться с заданной скоростью, проверять коллизии
 * с другими объектами и отрисовываться на экране.
 */
class ABullet
{
public:
    /**
     * Конструктор для инициализации пули.
     * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
     * @param ShootAt Тип цели, на которую направлена пуля.
     * @param StartPosition Начальная позиция пули.
     * @param RendererSprite Ссылка на менеджер спрайтов для отрисовки.
     */
    ABullet(const bool NewVelocity, const EBulletType& ShootAt, const sf::Vector2f& StartPosition,
            ASpriteManager& RendererSprite);

    /**
     * Деструктор для освобождения ресурсов.
     */
    ~ABullet();

    /**
     * Проверяет столкновение пули с целью.
     * @param TargetRect Ограничивающий прямоугольник цели для проверки столкновения.
     * @return true, если пуля сталкивается с целью, иначе false.
     */
    bool CheckCollision(const sf::FloatRect& TargetRect) const;

    /**
     * Обновляет позицию пули на основе прошедшего времени.
     * @param DeltaTime Время, прошедшее с последнего обновления.
     */
    void UpdateBulletPosition(float DeltaTime);

    /**
     * Возвращает урон, наносимый пулей.
     * @return Значение урона пули.
     */
    int GetBulletDamage() const;

    /**
     * Возвращает ограничивающий прямоугольник пули.
     * @return Ограничивающий прямоугольник пули.
     */
    sf::FloatRect GetBulletCollider() const;

    /**
     * Возвращает тип пули.
     * @return Тип пули (EBulletType).
     */
    EBulletType GetBulletType() const;

    /**
     * Отрисовывает пулю на указанном окне отрисовки.
     * @param Window Окно отрисовки, на котором отображается пуля.
     */
    void DrawBullet(sf::RenderWindow& Window) const;

private:
    int BulletDamage; // Урон, наносимый пулей.
    EBulletType BulletType; // Тип пули (игрок или враг).
    sf::Vector2f BulletVelocity; // Вектор скорости пули.
    sf::FloatRect BulletRect; // Прямоугольник столкновения пули.
    sf::Texture* BulletTexturePtr; // Указатель на текстуру пули.
    sf::Sprite BulletSprite; // Спрайт, представляющий пулю.
};
