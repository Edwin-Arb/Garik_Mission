#pragma once
#include "Constants.h"
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"

class AGameState
{
public:
    AGameState();
    ~AGameState();
    
    void InitGame() const;
    void UpdateGameplay(float DeltaTime);
    void DrawGame(sf::RenderWindow& Window) const;

private:
    // Для размеров окна игры
    sf::FloatRect ScreenRect;

    // Динамические классы
    AEnemy *Enemy;
    APlayer *Player;
    std::vector<ABullet*> BulletsVectorPtr;
 
    // Для создания интервала между выстрелами
    sf::Clock DelayShotTimerHandle;
};
