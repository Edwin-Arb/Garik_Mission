#pragma once
#include "../Bullet/Bullet.h"


class AEnemy
{
public:
    AEnemy();
    ~AEnemy();

    void InitEnemy(ASpriteManager& RendererSprite);
    void CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
                                    const sf::Vector2f& EnemySize,
                                    const sf::Vector2f& DrawScale);

    sf::FloatRect GetEnemyRect() const;
    void DrawEnemy(sf::RenderWindow& Window);

private:
    sf::Vector2f EnemyVelocity;
    sf::FloatRect EnemyRect;

    // Переменная для хранения позиции отрисовки врага
    sf::Vector2f EnemyDrawPosition;

    sf::Texture* EnemyTexturePtr;
    sf::Sprite EnemySprite;

    // Нужен для тестирование коллизий, удалить после тестов
    sf::RectangleShape EnemyRectCollision;
};
