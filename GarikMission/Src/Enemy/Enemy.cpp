#include "Enemy.h"

AEnemy::AEnemy()
    : EnemyVelocity{0.f, 0.f},
      EnemyRect{300.f, 500.f, 300.f + ENEMY_SIZE.x, 500.f + ENEMY_SIZE.y},
      EnemyTexturePtr(new sf::Texture),
      EnemyRectCollision(sf::Vector2f{ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y})
{}

AEnemy::~AEnemy()
{
    delete EnemyTexturePtr;
}

void AEnemy::InitEnemy(IRendererSprite& RendererSprite)
{
    // Подгрузить текстуру из папки для персонажа
    assert(EnemyTexturePtr->loadFromFile(RESOURCES_PATH + "MainTiles/enemy.png"));
    
    // Создать спрайт персонажа и положение на карте
    EnemySprite.setTexture(*EnemyTexturePtr);
    EnemySprite.setTextureRect(sf::IntRect(4, 1,
                               static_cast<int>(ENEMY_SIZE.x),
                               static_cast<int>(ENEMY_SIZE.y)));

    // Установить масштаб персонажа
    RendererSprite.SetSpriteSize(EnemySprite, ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y);
    RendererSprite.SetShapeSize(EnemyRectCollision, ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y);

    // Установить центр спрайта и коллизии
    RendererSprite.SetSpriteRelativeOrigin(EnemySprite, 0.5f, 0.5f);
    RendererSprite.SetShapeRelativeOrigin(EnemyRectCollision, 0.5f, 0.5f);
}

sf::FloatRect AEnemy::GetEnemyRect() const
{
    return EnemyRect;
}

void AEnemy::DrawEnemy(sf::RenderWindow &Window)
{
    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    EnemySprite.setPosition(EnemyRect.left,EnemyRect.top);
    EnemyRectCollision.setPosition(EnemyRect.left, EnemyRect.top);

    // Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    EnemyRectCollision.setFillColor(sf::Color(255, 0, 0,   128));
    
    Window.draw(EnemyRectCollision);
    Window.draw(EnemySprite);
}