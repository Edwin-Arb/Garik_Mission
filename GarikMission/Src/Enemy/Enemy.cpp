#include "Enemy.h"
#include "../Manager/CollisionManager.h"

/**
 * @brief Конструктор врага.
 * 
 * @param StartPosition Начальная позиция врага.
 */
AEnemy::AEnemy(const float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : bIsMoveRight(true)
      , bIsPlayerDetected(false)
      , EnemyScale(1.f)
      , MinMoveDistance(0.f)
      , MaxMoveDistance(MaxPatrolDistance)
      , EnemyStartPosition(StartPosition)
      , LineTraceDetectionArea(sf::Vector2f{200.f, 10.f}) // Ширина и высота detection area
{
    PawnSpeed = ENEMY_SPEED;

    PawnMaxHealth = ENEMY_MAX_HEALTH;
    PawnCurrentHealth = ENEMY_MAX_HEALTH;

    //  ActorCollisionRect = {
    //      StartPosition.x, StartPosition.y,
    //      (ENEMY_SIZE.x * DRAW_SCALE.x),
    //      (ENEMY_SIZE.y * DRAW_SCALE.y)
    //  };
    //
    // ActorSize = {ENEMY_SIZE.x, ENEMY_SIZE.y};
}

/**
 * @brief Инициализация врага.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void AEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // Инициализация переменных для текстуры врага
    // const std::string EnemyTexturePath = ASSETS_PATH + "MainTiles/Enemy.png";
    // const sf::IntRect EnemyRectTexture = sf::IntRect(4, 1, static_cast<int>(ENEMY_SIZE.x),
    //                                                  static_cast<int>(ENEMY_SIZE.y));
    // const sf::Vector2f EnemySize = {ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y};
    // const sf::Vector2f EnemyOrigin = {0.5f, 0.5f};
    //
    // // Инициализировать текстуру для врага и создания спрайта для него.
    // InitActorTexture(EnemyTexturePath, EnemyRectTexture, EnemySize, EnemyOrigin, SpriteManager);

    // WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    // WalkAnimation.FrameSpeed = 5.f;
    // WalkAnimation.FrameRect.emplace_back(EnemyRectTexture);
    // WalkAnimation.FrameRect.emplace_back(20, 1, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));

    // Установить центр спрайта и коллизии
    SpriteManager.SetShapeRelativeOrigin(LineTraceDetectionArea, 0.5f, 0.5f);

    // Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    LineTraceDetectionArea.setFillColor(sf::Color(155, 0, 0, 128));

    // Инициализировать шкалу здоровья врага
    InitPawnHealthBar(sf::Vector2f(10.f, 2.f), sf::Vector2f(0.2f, 0.2f),
                      sf::Color::Green, sf::Color::Red,
                      SpriteManager);

    //EnemyHealthBarPtr->InitHealthBar(TODO, sf::Color::Green, sf::Color::Red, SpriteManager);
}

/**
 * @brief Враг открывает огонь в сторону персонажа.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void AEnemy::EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager)
{
    // Выстрел из оружия, если персонаж обнаружен
    if (bIsPlayerDetected)
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? 10.f : 0.f;
        constexpr float SpawnBulletOffsetY = 9.f;
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, EBulletType::EBT_ShootAtPlayer,
                                                  sf::Vector2f(ActorCollisionRect.left + SpawnBulletOffsetX,
                                                               ActorCollisionRect.top + SpawnBulletOffsetY),
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
void AEnemy::CalculateEnemyDrawPosition()
{
    ActorDrawPosition =
    {
        ActorCollisionRect.left + (ActorSize.x * DRAW_SCALE.x) / 2.f,
        ActorCollisionRect.top + (ActorSize.y * DRAW_SCALE.y) / 2.f
    };
}

/**
 * @brief Обнаружение персонажа в окрестности.
 * 
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 */
void AEnemy::DetectPlayer(APlayer& Player, const AGameMap& GameMap)
{
    // Изначально предполагаем, что персонаж не обнаружен
    bIsPlayerDetected = false;

    // Проверка обнаружения персонажа
    if (LineTraceDetectionArea.getGlobalBounds().intersects(Player.GetActorCollisionRect()))
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
void AEnemy::UpdateDirectionAndVelocity(float DeltaTime, APlayer& Player)
{
    ActorVelocity.y += GRAVITY * DeltaTime;

    // Если персонаж обнаружен и не блокируется препятствием, разворачиваем врага в сторону персонажа
    if (bIsPlayerDetected)
    {
        ActorVelocity.x = 0.f;

        // Определяем направление персонажа относительно врага
        if (Player.GetActorCollisionRect().left < ActorCollisionRect.left)
        {
            // персонаж слева от врага
            bIsMoveRight = false;
            ActorSprite.setScale(-EnemyScale * DRAW_SCALE.x, EnemyScale * DRAW_SCALE.y);
        }
        else
        {
            // персонаж справа от врага
            bIsMoveRight = true;
            ActorSprite.setScale(EnemyScale * DRAW_SCALE.x, EnemyScale * DRAW_SCALE.y);
        }
    }
    else
    {
        // Движение влево или вправо в зависимости от текущего направления    
        if (bIsMoveRight)
        {
            ActorVelocity.x = PawnSpeed * DeltaTime;
            ActorSprite.setScale(EnemyScale * DRAW_SCALE.x, EnemyScale * DRAW_SCALE.y);
        }
        else
        {
            ActorVelocity.x = -PawnSpeed * DeltaTime;
            ActorSprite.setScale(-EnemyScale * DRAW_SCALE.x, EnemyScale * DRAW_SCALE.y);
        }
        //ActorSprite.setTextureRect(WalkAnimation.GetCurrentFrame());
        //WalkAnimation.AnimationUpdate(DeltaTime);
    }
}

/**
 * @brief Обновление позиции врага на основе текущей скорости.
 */
void AEnemy::UpdatePosition()
{
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;
    // EnemyRect.width = EnemyRect.left + (ENEMY_SIZE.x * DRAW_SCALE.x);
    // EnemyRect.height = EnemyRect.top + (ENEMY_SIZE.y * DRAW_SCALE.y);
}

/**
 * @brief Обновление позиции зоны обнаружения персонажа.
 */
void AEnemy::UpdateDetectionAreaPosition()
{
    sf::Vector2f DetectionPosition = ActorDrawPosition;
    if (bIsMoveRight)
    {
        DetectionPosition.x += (ActorSize.x * DRAW_SCALE.x) / 2.f;
    }
    else
    {
        DetectionPosition.x -= (ActorSize.x * DRAW_SCALE.x) / 2.f + LineTraceDetectionArea.getSize().x;
    }
    LineTraceDetectionArea.setPosition(DetectionPosition);
}

/**
 * @brief Обновление дистанции движения врага.
 */
void AEnemy::UpdateMoveDistance()
{
    // Проверка достижения максимальной или минимальной дистанции
    const float DistanceMoved = ActorCollisionRect.left - EnemyStartPosition.x;
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
void AEnemy::UpdateEnemyMove(float DeltaTime, APlayer& Player,
                             const AGameMap& GameMap,
                             const ACollisionManager& CollisionManager)
{
    DetectPlayer(Player, GameMap);
    UpdateDirectionAndVelocity(DeltaTime, Player);
    UpdatePosition();
    UpdateDetectionAreaPosition();
    UpdateMoveDistance();

    CollisionManager.HandlePawnCollisionWithGameMap(ActorCollisionRect, ActorVelocity, bIsPawnJump, bIsPawnJump);

    // TODO: Нужна, чтобы установить положение каждого врага отдельно.
    // TODO: Позже переместиться в постоянную обработку Геймплея игры
    CalculateEnemyDrawPosition();

    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    ActorSprite.setPosition(ActorDrawPosition);
    LineTraceDetectionArea.setPosition(ActorDrawPosition);

    // TODO: test health
    // Устанавливаем положение шкалы здоровья чуть выше врага
    UpdatePawnHealthBar(PawnCurrentHealth, PawnMaxHealth, sf::Vector2f(ActorDrawPosition - sf::Vector2f(0.f, 10.f)));
}

/**
* @brief Отрисовка врага на экране.
* 
* @param Window Окно, в котором происходит отрисовка.
*/
void AEnemy::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(LineTraceDetectionArea);
    Window.draw(ActorSprite);

    // Отрисовать шкалу здоровья врага
    PawnHealthBarPtr->Draw(Window);
}
