#include "Enemy.h"
#include "../Manager/CollisionManager.h"

/**
 * @brief Конструктор врага.
 * 
 * @param EnemyMaxHealth Максимальное здоровье врага.
 * @param StartPosition Начальная позиция врага.
 */
AEnemy::AEnemy(const float EnemyMaxHealth, const sf::Vector2f& StartPosition)
    : bIsMoveRight(true)
    , bIsPlayerDetected(false)
    , EnemyHealth(EnemyMaxHealth)
    , MinMoveDistance(100.f)
    , MaxMoveDistance(400.f)
    , EnemyStartPosition(StartPosition)
    //, Velocity{0.f, 0.f}
    , EnemyRect{StartPosition.x, StartPosition.y,
                (ENEMY_SIZE.x * DRAW_SCALE.x),
                (ENEMY_SIZE.y * DRAW_SCALE.y)}
    , EnemyTexturePtr(new sf::Texture)
    , LineTraceDetectionArea(sf::Vector2f{400.f, 100.f}) // Ширина и высота detection area
    , EnemyHealthBarPtr(new AHealthBar)
{
}

/**
 * @brief Деструктор врага.
 */
// AEnemy::~AEnemy()
// {
//     delete EnemyTexturePtr;
//     delete EnemyHealthBarPtr;
// }

/**
 * @brief Инициализация врага.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void AEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // Подгрузить текстуру из папки для персонажа
    assert(EnemyTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/enemy.png"));

    // Создать спрайт персонажа и установить его текстуру и прямоугольник
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

    // Инициализировать шкалу здоровья врага
    EnemyHealthBarPtr->InitHealthBar(100, 20, sf::Color::Green, sf::Color::Red, SpriteManager);
}

/**
 * @brief Враг открывает огонь в сторону персонажа.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void AEnemy::EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const
{
    // Выстрел из оружия, если персонаж обнаружен
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

/**
 * @brief Вычисление позиции отрисовки врага.
 * 
 * @param EnemyRectRef Прямоугольник коллизии врага.
 * @param EnemySize Размеры врага.
 * @param DrawScale Масштаб отрисовки.
 */
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

/**
 * @brief Обнаружение персонажа в окрестности.
 * 
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 */
void AEnemy::DetectPlayer(const APlayer& Player, const AGameMap& GameMap)
{
    // Изначально предполагаем, что персонаж не обнаружен
    bIsPlayerDetected = false;

    // Проверка обнаружения персонажа
    if (LineTraceDetectionArea.getGlobalBounds().intersects(Player.GetPlayerRect()))
    {
        bIsPlayerDetected = true;
        for (const auto& Obstacle : GameMap.GetGameMapCollisionVector())
        {
            if (LineTraceDetectionArea.getGlobalBounds().intersects(Obstacle))
            {
                bIsPlayerDetected = false;
                break;
            }
        }
    }
}

/**
 * @brief Обновление направления и скорости врага.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 */
void AEnemy::UpdateDirectionAndVelocity(float DeltaTime, const APlayer& Player)
{
    // Если персонаж обнаружен и не блокируется препятствием, разворачиваем врага в сторону персонажа
    if (bIsPlayerDetected)
    {
        Velocity.x = 0.f;

        // Определяем направление персонажа относительно врага
        if (Player.GetPlayerRect().left < EnemyRect.left)
        {
            // персонаж слева от врага
            bIsMoveRight = false;
            EnemySprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        else
        {
            // персонаж справа от врага
            bIsMoveRight = true;
            EnemySprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
    }
    else
    {
        // Движение влево или вправо в зависимости от текущего направления    
        if (bIsMoveRight)
        {
            Velocity.x = 200.f * DeltaTime;
            EnemySprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        else
        {
            Velocity.x = -200.f * DeltaTime;
            EnemySprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
    }
}

/**
 * @brief Обновление позиции врага на основе текущей скорости.
 */
void AEnemy::UpdatePosition()
{
    EnemyRect.left += Velocity.x;
    EnemyRect.top -= Velocity.y;
    // EnemyRect.width = EnemyRect.left + (ENEMY_SIZE.x * DRAW_SCALE.x);
    // EnemyRect.height = EnemyRect.top + (ENEMY_SIZE.y * DRAW_SCALE.y);
}

/**
 * @brief Обновление позиции зоны обнаружения персонажа.
 */
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

/**
 * @brief Обновление дистанции движения врага.
 */
void AEnemy::UpdateMoveDistance()
{
    // Проверка достижения максимальной или минимальной дистанции
    const float DistanceMoved = EnemyRect.left - EnemyStartPosition.x;
    if (DistanceMoved >= MaxMoveDistance || DistanceMoved <= -MinMoveDistance)
    {
        // Меняем направление движения только если персонаж не обнаружен
        if (!bIsPlayerDetected)
        {
            bIsMoveRight = !bIsMoveRight;
        }
    }
}

/**
 * @brief Обновление движения врага.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 */
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

    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    EnemySprite.setPosition(EnemyDrawPosition);
    LineTraceDetectionArea.setPosition(EnemyDrawPosition);

    // TODO test health
    // Устанавливаем положение шкалы здоровья чуть выше врага
    EnemyHealthBarPtr->SetPosition(EnemyDrawPosition - sf::Vector2f(0.f, 80.f));
    EnemyHealthBarPtr->Update(EnemyHealth, 400);
}

/**
 * @brief Установка здоровья врага.
 * 
 * @param Damage Полученный урон от пули.
 */
void AEnemy::SetEnemyHealth(int Damage)
{
    EnemyHealth -= Damage;
}

/**
 * @brief Получение текущего здоровья врага.
 * 
 * @return Текущее здоровье врага.
 */
float AEnemy::GetEnemyHealth() const
{
    return EnemyHealth;
}

/**
 * @brief Получение прямоугольника коллизии врага.
 * 
 * @return Прямоугольник коллизии врага.
 */
sf::FloatRect AEnemy::GetEnemyRect() const
{
    return EnemyRect;
}

void AEnemy::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(LineTraceDetectionArea);
    Window.draw(EnemySprite);

    // Отрисовать шкалу здоровья врага
    EnemyHealthBarPtr->Draw(Window);
}

/**
 * @brief Отрисовка врага на экране.
 * 
 * @param Window Окно, в котором происходит отрисовка.
 */
// void AEnemy::DrawEnemy(sf::RenderWindow& Window) const
// {
//     Window.draw(LineTraceDetectionArea);
//     Window.draw(EnemySprite);
//
//     // Отрисовать шкалу здоровья врага
//     EnemyHealthBarPtr->Draw(Window);
// }