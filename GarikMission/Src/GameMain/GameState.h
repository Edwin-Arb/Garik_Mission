#pragma once
#include "../Manager/CollisionManager.h"
#include "../Manager/FpsManager.h"


class AGameState
{
public:
    AGameState();
    ~AGameState();

    void InitGame();
    void UpdateGameplay(float DeltaTime);
    void UpdateInput(float DeltaTime);
    void UpdateCamera(sf::RenderWindow& Window);
    void DrawGame(sf::RenderWindow& Window) const;

private:
    // Для размеров окна игры
    sf::FloatRect ScreenRect;
    
    // Динамические классы для основы игры
    AEnemy* EnemyPtr;
    APlayer* PlayerPtr;
    AGameMap *GameMapPtr;

    // Для обработки страйта объектов
    ASpriteManager* SpriteManagerPtr;

    // Для проверок коллизии
    ACollisionManager* CollisionManagerPtr;

    // Для отображения кадров в игре
    AFpsManager *FpsManagerPtr;
    
    // Массив укзателей на пули
    std::vector<ABullet*> BulletsVectorPtr;

    // Для создания интервала между выстрелами
    sf::Clock DelayShotTimerHandle;

    // Камера, чтобы сфокусироваться на игроке
    sf::View ViewPlayer;
};
