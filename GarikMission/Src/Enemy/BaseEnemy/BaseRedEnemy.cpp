#include "BaseRedEnemy.h"

ABaseRedEnemy::ABaseRedEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
{
}

void ABaseRedEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    AEnemy::InitEnemy(SpriteManager);

    // Инициализация переменных для текстуры врага
    const sf::IntRect EnemyRectTexture = sf::IntRect(4, 1, static_cast<int>(ENEMY_SIZE.x),
                                                           static_cast<int>(ENEMY_SIZE.y));

    // Инициализировать текстуру для врага и создания спрайта для него.
    InitActorTexture(EnemyTexturePath, EnemyRectTexture, ActorSize, EnemyOrigin, SpriteManager);

    WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    WalkAnimation.FrameSpeed = 5.f;
    WalkAnimation.FrameRect.emplace_back(EnemyRectTexture);
    WalkAnimation.FrameRect.emplace_back(20, 1, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));
}
