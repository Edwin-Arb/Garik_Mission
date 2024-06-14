#pragma once
#include "../Manager/CollisionManager.h"
#include "../Manager/FpsManager.h"


/**
 * @brief Класс AGameState представляет текущее состояние игры.
 * Он управляет инициализацией игры, обновлением ввода, игрового процесса, камеры и отрисовкой.
 */
class AGameState
{
public:
    /**
     * @brief Конструктор класса AGameState.
     */
    AGameState();

    /**
     * @brief Деструктор класса AGameState.
     */
    ~AGameState();

    /**
     * @brief Инициализирует начальное состояние игры, включая игровые объекты и менеджеры.
     */
    void InitGame();

    /**
     * @brief Обновляет ввод пользователя.
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateInput(float DeltaTime);

    /**
     * @brief Обновляет игровой процесс, включая физику и логику объектов.
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateGameplay(float DeltaTime);

    /**
     * @brief Обновляет камеру игры в зависимости от положения игрока.
     * @param Window Окно для отрисовки игры.
     */
    void UpdateCamera(sf::RenderWindow& Window);

    /**
     * @brief Отрисовывает текущее состояние игры в окне.
     * @param Window Окно для отрисовки игры.
     */
    void DrawGame(sf::RenderWindow& Window) const;

private:
    sf::FloatRect ScreenRect;                       // Для размеров окна игры
 
    APlayer* PlayerPtr;                             // Динамические классы для основы игры
    AGameMap *GameMapPtr;
    std::vector<AEnemy*> EnemysVectorPtr;
    std::vector<ABullet*> BulletsVectorPtr;
 
    ASpriteManager* SpriteManagerPtr;               // Для обработки страйта объектов
    ACollisionManager* CollisionManagerPtr;         // Для проверок коллизии
    AFpsManager *FpsManagerPtr;                     // Для отображения кадров в игре
 
    sf::Clock DelayShotTimerHandle;                 // Для создания интервала между выстрелами
    sf::View ViewPlayer;                            // Камера, чтобы сфокусироваться на игроке
};
