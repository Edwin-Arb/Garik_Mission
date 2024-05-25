#pragma once
#include "../Player/Player.h"
#include "../Manager/InputManager.h"
#include "../Manager/SpriteManager.h"


class AGameState
{
public:
    AGameState();
    ~AGameState();

    void InitGame();
    void UpdateGameplay(float DeltaTime);
    void DrawGame(sf::RenderWindow& Window) const;

private:
    // Для размеров окна игры
    sf::FloatRect ScreenRect;

    // Динамические классы
    AEnemy* EnemyPtr;
    APlayer* PlayerPtr;

    // Рендер движки объектов
    AInputManager* InputManagerPtr;
    ASpriteManager* SpriteManagerPtr;

    // Массив укзателей на пули
    std::vector<ABullet*> BulletsVectorPtr;

    // Для создания интервала между выстрелами
    sf::Clock DelayShotTimerHandle;
};
