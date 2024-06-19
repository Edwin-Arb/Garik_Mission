#pragma once
#include "../Bullet/Bullet.h"
#include "../Player/Player.h"
#include "../GameMap/GameMap.h"


/**
 * @brief Класс, представляющий врага в игре.
 */
class AEnemy : public APawn
{
public:
    /**
     * @brief Конструктор врага.
     * 
     * @param EnemyMaxHealth Максимальное здоровье врага.
     * @param StartPosition Начальная позиция врага.
     */
    AEnemy(float EnemyMaxHealth, const sf::Vector2f& StartPosition);

    /**
     * @brief Деструктор врага.
     */
    ~AEnemy() override = default;
    
    /**
     * @brief Инициализация врага.
     * 
     * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
     */
     void InitEnemy(ASpriteManager &SpriteManager);

    /**
     * @brief Враг открывает огонь.
     * 
     * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
     * @param SpriteManager Менеджер спрайтов для управления спрайтами.
     */
    void EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const;

    /**
     * @brief Вычисление позиции отрисовки врага.
     * 
     * @param EnemyRectRef Прямоугольник коллизии врага.
     * @param EnemySize Размеры врага.
     * @param DrawScale Масштаб отрисовки.
     */
    void CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
                                    const sf::Vector2f& EnemySize,
                                    const sf::Vector2f& DrawScale);

    /**
     * @brief Обнаружение персонажаа в окрестности.
     * 
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     */
    void DetectPlayer(APlayer& Player, const AGameMap& GameMap);

    /**
     * @brief Обновление направления и скорости врага.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     * @param Player Ссылка на объект персонажа.
     */
    void UpdateDirectionAndVelocity(float DeltaTime, APlayer& Player);

    /**
     * @brief Обновление позиции врага на основе текущей скорости.
     */
    void UpdatePosition();

    /**
     * @brief Обновление позиции зоны обнаружения персонажа.
     */
    void UpdateDetectionAreaPosition();

    /**
     * @brief Обновление дистанции, на которую может двигаться враг.
     */
    void UpdateMoveDistance();

    /**
     * @brief Обновление движения врага.
     * 
     * @param DeltaTime Время, прошедшее с последнего обновления.
     * @param Player Ссылка на объект персонажа.
     * @param GameMap Ссылка на объект игровой карты.
     */
    void UpdateEnemyMove(float DeltaTime, APlayer& Player, const AGameMap& GameMap, const ACollisionManager &CollisionManager);
 
    /**
     * @brief Отрисовка врага на экране.
     * 
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawActor(sf::RenderWindow &Window) override;

private:
    bool bIsMoveRight;                             // Флаг направления движения вправо.
    bool bIsPlayerDetected;                        // Флаг обнаружения персонажа.
 
    float MinMoveDistance;                         // Минимальная дистанция движения врага.
    float MaxMoveDistance;                         // Максимальная дистанция движения врага.

    sf::Vector2f EnemyStartPosition;               // Начальная позиция врага.
 
    // TODO: Нужен для тестирования обнаружения персонажа, удалить после тестов
    sf::RectangleShape LineTraceDetectionArea;     // Прямоугольная область для тестирования обнаружения.
};