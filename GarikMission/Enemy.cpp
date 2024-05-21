#include "Enemy.h"

AEnemy::AEnemy()
{
    enemyVelocity = {0.f, 0.f};
    enemyRect = {650.f, 400.f,
                   PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.x};
}

void AEnemy::InitEnemy()
{
    // Подгрузить текстуру из папки для персонажа
    enemyTexture.loadFromFile(RESOURCES_PATH + "MainTiles/enemy.png");
    
    // Создать спрайт персонажа и положение на карте
    enemySprite.setTexture(enemyTexture);
    enemySprite.setTextureRect(sf::IntRect(4, 1,
                               static_cast<int>(PLAYER_SIZE.x),
                               static_cast<int>(PLAYER_SIZE.y)));
    enemySprite.setScale(-DRAW_SCALE.x, DRAW_SCALE.y);
}

void AEnemy::DrawEnemy(sf::RenderWindow &window)
{
    enemySprite.setPosition(enemyRect.left, enemyRect.top);
    window.draw(enemySprite);
}