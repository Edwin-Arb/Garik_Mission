#include "Enemy.h"
#include "../Manager/CollisionManager.h"


AEnemy::AEnemy(const sf::Vector2f StartPosition)
    : bMovingRight(true)
      , bPlayerDetected(false)
      , MinMoveDistance(100.f)
      , MaxMoveDistance(400.f)
      , EnemyStartPosition(StartPosition)
      , EnemyVelocity{0.f, 0.f}
      , EnemyRect
      {
          StartPosition.x, StartPosition.y,
          StartPosition.x + (ENEMY_SIZE.x * DRAW_SCALE.x),
          StartPosition.y + (ENEMY_SIZE.y * DRAW_SCALE.y)
      }
      , EnemyTexturePtr(new sf::Texture)
      , LineTraceDetectionArea(sf::Vector2f{300.f, 5.f})
{
}

AEnemy::~AEnemy()
{
    delete EnemyTexturePtr;
}

void AEnemy::InitEnemy(ASpriteManager& RendererSprite, float MaxDistanceMove)
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
    RendererSprite.SetShapeSize(LineTraceDetectionArea, 300.f, 5.f);

    // Установить центр спрайта и коллизии
    RendererSprite.SetSpriteRelativeOrigin(EnemySprite, 0.5f, 0.5f);
    RendererSprite.SetShapeRelativeOrigin(LineTraceDetectionArea, 0.f, 0.f);

    // Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    LineTraceDetectionArea.setFillColor(sf::Color(255, 0, 0, 128));
}

void AEnemy::CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef,
                                        const sf::Vector2f& EnemySize,
                                        const sf::Vector2f& DrawScale)
{
    EnemyDrawPosition =
    {
        EnemyRectRef.left + (EnemySize.x * DrawScale.x) / 2.f,
        EnemyRectRef.top + (EnemySize.y * DrawScale.y) / 2.f
    };
}

// void AEnemy::SetMovingRight(bool bRight)
// {
//     bMovingRight = bRight;
// }
//
// void AEnemy::SetMaxMoveDistance(float Distance)
// {
//     MaxMoveDistance = Distance;
// }
void AEnemy::UpdateEnemyMove(float DeltaTime, const APlayer& Player)
{
    // Проверка обнаружения игрока
    if (LineTraceDetectionArea.getGlobalBounds().intersects(Player.GetPlayerRect()))
    {
        EnemyVelocity.x = 0.f;
        bPlayerDetected = true;
    }
    else
    {
        bPlayerDetected = false;
    }

    if (!bPlayerDetected)
    {
        // Движение влево или вправо в зависимости от текущего направления    
        if (bMovingRight)
        {
            EnemyVelocity.x = 200.f * DeltaTime;
            EnemySprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
            LineTraceDetectionArea.setScale(1.f, 1.f); // Лайн трейс справа
        }
        else
        {
            EnemyVelocity.x = -200.f * DeltaTime;
            EnemySprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
            LineTraceDetectionArea.setScale(-1.f, 1.f); // Лайн трейс слева
        }

        // Проверка достижения максимальной или минимальной дистанции
        const float DistanceMoved = EnemyRect.left - EnemyStartPosition.x;
        if (DistanceMoved >= MaxMoveDistance || DistanceMoved <= -MinMoveDistance)
        {
            // Меняем направление движения только если игрок не обнаружен
            bMovingRight = !bMovingRight;
        }
    }

    // Обновление позиции врага
    EnemyRect.left += EnemyVelocity.x;
    EnemyRect.width = EnemyRect.left + ENEMY_SIZE.x * DRAW_SCALE.x;
    EnemyRect.top -= EnemyVelocity.y;
    EnemyRect.height = EnemyRect.top + ENEMY_SIZE.y * DRAW_SCALE.y;

    // Обновление позиции зоны обнаружения игрока
    // sf::Vector2f DetectionPosition = EnemyDrawPosition;
    // if (bMovingRight)
    // {
    //     DetectionPosition.x += (ENEMY_SIZE.x * DRAW_SCALE.x) / 2.f;
    // }
    // else
    // {
    //     DetectionPosition.x -= (ENEMY_SIZE.x * DRAW_SCALE.x) / 2.f + LineTraceDetectionArea.getSize().x;
    // }
    // LineTraceDetectionArea.setPosition(DetectionPosition);

    // TODO Нужна, чтобы установить положение каждого врага отдельно.
    // TODO Позже переместиться в постоянную обработку Геймплея игры
    CalculateEnemyDrawPosition(EnemyRect, ENEMY_SIZE, DRAW_SCALE);
}


sf::FloatRect AEnemy::GetEnemyRect() const
{
    return EnemyRect;
}

void AEnemy::DrawEnemy(sf::RenderWindow& Window)
{
    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    EnemySprite.setPosition(EnemyDrawPosition);
    LineTraceDetectionArea.setPosition(EnemyDrawPosition);

    Window.draw(LineTraceDetectionArea);
    Window.draw(EnemySprite);
}
