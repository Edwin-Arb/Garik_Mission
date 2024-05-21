#pragma once
#include "Constants.h"
#include "Enemy.h"
#include "GameEngine.h"
#include "Player.h"

class AGameState
{
public:
    AGameState();
    //~AGameState();
    
    void InitGame();
    void UpdateGameplay(const sf::Event& event, float deltaTime);
    void DrawGame(sf::RenderWindow &window);

private:
    Rectangle screenRect;
    AEnemy enemy;
    APlayer player;
};
