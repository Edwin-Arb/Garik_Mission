#include "BaseEnemy.h"

ABaseEnemy::ABaseEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
{}

void ABaseEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    AEnemy::InitEnemy(SpriteManager);

    // Инициализация переменных для текстуры врага
    const std::string EnemyTexturePath = ASSETS_PATH + "MainTiles/Enemy.png";
    const sf::IntRect EnemyRectTexture = sf::IntRect(4, 1, static_cast<int>(ENEMY_SIZE.x),
                                                     static_cast<int>(ENEMY_SIZE.y));
    const sf::Vector2f EnemySize = {ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y};
    const sf::Vector2f EnemyOrigin = {0.5f, 0.5f};
    
    // Инициализировать текстуру для врага и создания спрайта для него.
    InitActorTexture(EnemyTexturePath, EnemyRectTexture, EnemySize, EnemyOrigin, SpriteManager);

    WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    WalkAnimation.FrameSpeed = 5.f;
    WalkAnimation.FrameRect.emplace_back(EnemyRectTexture);
    WalkAnimation.FrameRect.emplace_back(20, 1, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));
}
