#include "Enemy.h"
#include "../Manager/CollisionManager.h"


AEnemy::AEnemy(const int Health, const sf::Vector2f& StartPosition)
    : bIsMoveRight(true)
      , bIsPlayerDetected(false)
      , EnemyHealth(Health)
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
      , LineTraceDetectionArea(sf::Vector2f{400.f, 100.f}) // Ширина и высота detection area
      , EnemyHealthBarPtr(new AHealthBar)
{
}

AEnemy::~AEnemy()
{
    delete EnemyTexturePtr;
    delete EnemyHealthBarPtr;
}

// Инициализация врага
void AEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // Подгрузить текстуру из папки для персонажа
    assert(EnemyTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/enemy.png"));

    // Создать спрайт персонажа и положение на карте
    EnemySprite.setTexture(*EnemyTexturePtr);
    EnemySprite.setTextureRect(sf::IntRect(4, 1,
                                           static_cast<int>(ENEMY_SIZE.x),
                                           static_cast<int>(ENEMY_SIZE.y)));

    // Установить масштаб персонажа
    SpriteManager.SetSpriteSize(EnemySprite, ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y);

    // Установить центр спрайта и коллизии
    SpriteManager.SetSpriteRelativeOrigin(EnemySprite, 0.5f, 0.5f);
    SpriteManager.SetShapeRelativeOrigin(LineTraceDetectionArea, 0.5f, 0.5f);

    // Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    LineTraceDetectionArea.setFillColor(sf::Color(155, 0, 0, 128));

    // Инициализируем шкалу здоровья врага
    EnemyHealthBarPtr->InitHealthBar(100.f, 20.f, sf::Color::Green, sf::Color::Red, SpriteManager);
}

void AEnemy::EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const
{
    // Выстрел из оружия. Пока персонаж в зоне обнаружения врага
    if (bIsPlayerDetected)
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? 85.f : 0.f;
        constexpr float SpawnBulletOffsetY = 75.f;
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, EBulletType::EBT_ShootAtPlayer,
                                                  sf::Vector2f(EnemyRect.left + SpawnBulletOffsetX,
                                                               EnemyRect.top + SpawnBulletOffsetY),
                                                  SpriteManager));
    }
}

// Вычисление позиции отрисовки врага
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

// Обнаружение игрока
void AEnemy::DetectPlayer(const APlayer& Player, const AGameMap& GameMap)
{
    // Изначально предполагаем, что игрок не обнаружен
    bIsPlayerDetected = false;

    // Проверка обнаружения игрока
    if (LineTraceDetectionArea.getGlobalBounds().intersects(Player.GetPlayerRect()))
    {
        bIsPlayerDetected = true;
        for (const auto& Obstacle : GameMap.GetCollisionVector())
        {
            if (LineTraceDetectionArea.getGlobalBounds().intersects(Obstacle))
            {
                bIsPlayerDetected = false;
                break;
            }
        }
    }
}

// Обновление направления и скорости врага
void AEnemy::UpdateDirectionAndVelocity(float DeltaTime, const APlayer& Player)
{
    // Если игрок обнаружен и не блокируется препятствием, разворачиваем врага в сторону игрока
    if (bIsPlayerDetected)
    {
        EnemyVelocity.x = 0.f;

        // Определяем направление игрока относительно врага
        if (Player.GetPlayerRect().left < EnemyRect.left)
        {
            // Игрок слева от врага
            bIsMoveRight = false;
            EnemySprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        else
        {
            // Игрок справа от врага
            bIsMoveRight = true;
            EnemySprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
    }
    else
    {
        // Движение влево или вправо в зависимости от текущего направления    
        if (bIsMoveRight)
        {
            EnemyVelocity.x = 200.f * DeltaTime;
            EnemySprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        else
        {
            EnemyVelocity.x = -200.f * DeltaTime;
            EnemySprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
    }
}

// Обновление позиции врага
void AEnemy::UpdatePosition()
{
    EnemyRect.left += EnemyVelocity.x;
    EnemyRect.width = EnemyRect.left + (ENEMY_SIZE.x * DRAW_SCALE.x);
    EnemyRect.top -= EnemyVelocity.y;
    EnemyRect.height = EnemyRect.top + (ENEMY_SIZE.y * DRAW_SCALE.y);
}

// Обновление позиции зоны обнаружения игрока
void AEnemy::UpdateDetectionAreaPosition()
{
    sf::Vector2f DetectionPosition = EnemyDrawPosition;
    if (bIsMoveRight)
    {
        DetectionPosition.x += (ENEMY_SIZE.x * DRAW_SCALE.x) / 2.f;
    }
    else
    {
        DetectionPosition.x -= (ENEMY_SIZE.x * DRAW_SCALE.x) / 2.f + LineTraceDetectionArea.getSize().x;
    }
    LineTraceDetectionArea.setPosition(DetectionPosition);
}

// Обновление дистанции движения врага
void AEnemy::UpdateMoveDistance()
{
    // Проверка достижения максимальной или минимальной дистанции
    const float DistanceMoved = EnemyRect.left - EnemyStartPosition.x;
    if (DistanceMoved >= MaxMoveDistance || DistanceMoved <= -MinMoveDistance)
    {
        // Меняем направление движения только если игрок не обнаружен
        if (!bIsPlayerDetected)
        {
            bIsMoveRight = !bIsMoveRight;
        }
    }
}

// Обновление движения врага
void AEnemy::UpdateEnemyMove(float DeltaTime, const APlayer& Player, const AGameMap& GameMap)
{
    DetectPlayer(Player, GameMap);
    UpdateDirectionAndVelocity(DeltaTime, Player);
    UpdatePosition();
    UpdateDetectionAreaPosition();
    UpdateMoveDistance();

    // TODO Нужна, чтобы установить положение каждого врага отдельно.
    // TODO Позже переместиться в постоянную обработку Геймплея игры
    CalculateEnemyDrawPosition(EnemyRect, ENEMY_SIZE, DRAW_SCALE);


    // TODO test health
    // Устанавливаем положение шкалы здоровья чуть выше врага
    EnemyHealthBarPtr->SetPosition(EnemyDrawPosition - sf::Vector2f(0.f, 80.f));
    EnemyHealthBarPtr->Update(EnemyHealth, 400);
}

// Установка здоровья врага
void AEnemy::SetEnemyHealth(int Damage)
{
    EnemyHealth -= Damage;
}

// Получение здоровья врага
int AEnemy::GetEnemyHealth() const
{
    return EnemyHealth;
}

// Получение прямоугольника(коллизии) врага
sf::FloatRect AEnemy::GetEnemyRect() const
{
    return EnemyRect;
}

// Отрисовка врага
void AEnemy::DrawEnemy(sf::RenderWindow& Window)
{
    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    EnemySprite.setPosition(EnemyDrawPosition);
    LineTraceDetectionArea.setPosition(EnemyDrawPosition);

    Window.draw(LineTraceDetectionArea);
    Window.draw(EnemySprite);

    // Отрисовать шкалу здоровья врага
    EnemyHealthBarPtr->Draw(Window);
}
