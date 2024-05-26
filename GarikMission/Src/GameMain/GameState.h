#pragma once
#include "../Manager/CollisionManager.h"


class AGameState
{
public:
    AGameState();
    ~AGameState();

    void InitGame();
    void UpdateGameplay(float DeltaTime);
    void UpdateInput(float DeltaTime);
    void DrawGame(sf::RenderWindow& Window) const;

private:
    // Для размеров окна игры
    sf::FloatRect ScreenRect;

    // Динамические классы для основы игры
    AEnemy* EnemyPtr;
    APlayer* PlayerPtr;

    //Для обработки страйта объектов
    ASpriteManager* SpriteManagerPtr;

    // Для проверок коллизии
    ACollisionManager* CollisionManagerPtr;

    // Массив укзателей на пули
    std::vector<ABullet*> BulletsVectorPtr;

    // Для создания интервала между выстрелами
    sf::Clock DelayShotTimerHandle;
};
