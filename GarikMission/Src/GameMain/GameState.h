#pragma once
#include "../Manager/CollisionManager.h"
#include "../Manager/FpsManager.h"


/**
 * Класс AGameState представляет текущее состояние игры.
 * Он управляет инициализацией игры, обновлением ввода, игрового процесса, камеры и отрисовкой.
 */
class AGameState
{
public:
    /**
     * Конструктор класса AGameState.
     */
    AGameState();

    /**
     * Деструктор класса AGameState.
     */
    ~AGameState();

    /**
     * Инициализирует начальное состояние игры, включая игровые объекты и менеджеры.
     */
    void InitGame();

    /**
     * Обновляет ввод пользователя.
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateInput(float DeltaTime);

    /**
     * Обновляет игровой процесс, включая физику и логику объектов.
     * @param DeltaTime Время, прошедшее с последнего кадра.
     */
    void UpdateGameplay(float DeltaTime);

    /**
     * Обновляет камеру игры в зависимости от положения игрока.
     * @param Window Окно для отрисовки игры.
     */
    void UpdateCamera(sf::RenderWindow& Window);

    /**
     * Отрисовывает текущее состояние игры в окне.
     * @param Window Окно для отрисовки игры.
     */
    void DrawGame(sf::RenderWindow& Window) const;

private:
    // Для размеров окна игры
    sf::FloatRect ScreenRect;
    
    // Динамические классы для основы игры
    APlayer* PlayerPtr;
    AGameMap *GameMapPtr;
    std::vector<AEnemy*> EnemysVectorPtr;
    std::vector<ABullet*> BulletsVectorPtr;

    // Для обработки страйта объектов
    ASpriteManager* SpriteManagerPtr;

    // Для проверок коллизии
    ACollisionManager* CollisionManagerPtr;

    // Для отображения кадров в игре
    AFpsManager *FpsManagerPtr;
    
    // Для создания интервала между выстрелами
    sf::Clock DelayShotTimerHandle;

    // Камера, чтобы сфокусироваться на игроке
    sf::View ViewPlayer;
};
