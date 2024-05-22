#pragma once
#include "Constants.h"
#include "Enemy.h"
#include "Player.h"

class AGameState
{
public:
    AGameState();
    //~AGameState();
    
    void InitGame();
    void UpdateGameplay(const sf::Event& event, float delta_time);
    void DrawGame(sf::RenderWindow &window);

private:
    // Для размеров окна игры
    Rectangle screenRect;

    // Динамические классы
    AEnemy enemy;
    APlayer player;
    std::vector<ABullet> bulletsVector;

    // Для создания интервала между выстрелами
    sf::Clock delayShootTime;
};
