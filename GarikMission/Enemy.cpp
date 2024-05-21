#include "Enemy.h"

AEnemy::AEnemy()
    : enemyVelocity{0.f, 0.f},
      enemyRect{650.f, 400.f,
             ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y},
      enemyShapeCollision(sf::Vector2f{-ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y})
{}

void AEnemy::InitEnemy()
{
    // Подгрузить текстуру из папки для персонажа
    enemyTexture.loadFromFile(RESOURCES_PATH + "MainTiles/enemy.png");
    
    // Создать спрайт персонажа и положение на карте
    enemySprite.setTexture(enemyTexture);
    enemySprite.setTextureRect(sf::IntRect(4, 1,
                               static_cast<int>(ENEMY_SIZE.x),
                               static_cast<int>(ENEMY_SIZE.y)));

    // Установить масштаб персонажа
    enemySprite.setScale(-DRAW_SCALE.x, DRAW_SCALE.y);
}

void AEnemy::DrawEnemy(sf::RenderWindow &window)
{
    // Установить положение спрайта с положением врага в игре, т.е с его прямоугольником(коллизией)
    enemySprite.setPosition(enemyRect.left, enemyRect.top);
    enemyShapeCollision.setPosition(enemyRect.left, enemyRect.top);
    window.draw(enemyShapeCollision);
    window.draw(enemySprite);
}