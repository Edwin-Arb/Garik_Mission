#pragma once
#include "Constants.h"


class AEnemy
{
public:
    AEnemy();

    void InitEnemy();
    void DrawEnemy(sf::RenderWindow& window);

private:
    sf::Vector2f enemyVelocity;
    sf::FloatRect enemyRect;
    
    sf::Texture enemyTexture;
    sf::Sprite enemySprite;

    // Нужен для тестирование коллизий, удалить после тестов
    sf::RectangleShape enemyShapeCollision;
};
