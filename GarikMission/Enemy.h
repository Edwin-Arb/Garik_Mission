#pragma once
#include "Constants.h"


class AEnemy
{
public:
    AEnemy();
    ~AEnemy();

    void InitEnemy();
    sf::FloatRect GetEnemyRect() const;
    void DrawEnemy(sf::RenderWindow& Window);

private:
    sf::Vector2f EnemyVelocity;
    sf::FloatRect EnemyRect;
    
    sf::Texture *EnemyTexturePtr;
    sf::Sprite EnemySprite;

    // Нужен для тестирование коллизий, удалить после тестов
    sf::RectangleShape EnemyRectCollision;
};
