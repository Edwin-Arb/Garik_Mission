#include "Enemy.h"


AEnemy::AEnemy()
    : EnemyVelocity{0.f, 0.f},
      EnemyRect
      {
          450.f, 360.f,
          450.f + (ENEMY_SIZE.x * DRAW_SCALE.x),
          360.f + (ENEMY_SIZE.y * DRAW_SCALE.y)
      },
      EnemyTexturePtr(new sf::Texture),
      EnemyRectCollision(sf::Vector2f{ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y})
{
}

AEnemy::~AEnemy()
{
    delete EnemyTexturePtr;
}

void AEnemy::InitEnemy(ASpriteManager& RendererSprite)
{
    // Подгрузить текстуру из папки для персонажа
    assert(EnemyTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/enemy.png"));

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

    // TODO Нужна, чтобы установить положение каждого врага отдельно.
    // TODO Позже переместиться в постоянную обработку Геймплея игры
    CalculateEnemyDrawPosition(EnemyRect, ENEMY_SIZE, DRAW_SCALE);
}

void AEnemy::CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
                                        const sf::Vector2f& EnemySize,
                                        const sf::Vector2f& DrawScale)
{
    EnemyDrawPosition = {
        EnemyRectRef.left + (EnemySize.x * DrawScale.x) / 2.f,
        EnemyRectRef.top + (EnemySize.y * DrawScale.y) / 2.f
    };
}

sf::FloatRect AEnemy::GetEnemyRect() const
{
    return EnemyRect;
}

void AEnemy::DrawEnemy(sf::RenderWindow& Window)
{
    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    EnemySprite.setPosition(EnemyDrawPosition);
    EnemyRectCollision.setPosition(EnemyDrawPosition);
    
    // Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    EnemyRectCollision.setFillColor(sf::Color(255, 0, 0, 128));

    Window.draw(EnemyRectCollision);
    Window.draw(EnemySprite);
}
