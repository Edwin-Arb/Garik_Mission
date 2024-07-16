#include "Enemy.h"
#include "../Manager/CollisionManager.h"
#include "BossEnemy/BossEnemy.h"

/**
 * @brief Конструктор врага.
 * 
 * @param MaxPatrolDistance Максимальная дистанция, на которую может двигаться враг.
 * @param StartPosition Начальная позиция врага.
 */
AEnemy::AEnemy(const float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : bIsMoveRight(true)
      , bIsPlayerDetected(false)
      , EnemyScale(1.f)
      , MinMoveDistance(MaxPatrolDistance / 2.f)
      , MaxMoveDistance(MaxPatrolDistance / 2.f)
      , EnemyTexturePath(ASSETS_PATH + "MainTiles/Enemy.png")
      , EnemyOrigin(0.5f, 0.5f)
      , EnemyHealthBarSize(10.f, 2.f)
      , EnemyStartPosition(StartPosition)
      , LineTraceDetectionArea(sf::Vector2f{200.f, 10.f}) // Ширина и высота detection area
{
    PawnSpeed = ENEMY_SPEED;

    PawnMaxHealth = ENEMY_MAX_HEALTH;
    PawnCurrentHealth = ENEMY_MAX_HEALTH;

    ActorSize = {ENEMY_SIZE.x * DRAW_SCALE.x, ENEMY_SIZE.y * DRAW_SCALE.y};

    ActorCollisionRect = {
        StartPosition.x, StartPosition.y,
        ActorSize.x, ActorSize.y
    };
}

/**
 * @brief Инициализация врага.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void AEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // TODO: Установить центр спрайта и коллизии для ЛайнТрейса (отключить после проверки)
    SpriteManager.SetShapeRelativeOrigin(LineTraceDetectionArea, 0.5f, 0.5f);

    // TODO: Отрисовать прямоугольник коллизии для визуализации (отключить после проверки)
    LineTraceDetectionArea.setFillColor(sf::Color(155, 0, 0, 128));

    // Инициализация шкалы здоровья врага
    InitPawnHealthBar(EnemyHealthBarSize,
                      sf::Color::Red, sf::Color(181, 184, 177),
                      SpriteManager);
}

/**
 * @brief Враг открывает огонь в сторону персонажа.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void AEnemy::EnemyShoot(const float SpawnBulletOffsetY, std::vector<ABullet*>& BulletsVectorPtr,
                        ASpriteManager& SpriteManager)
{
    // Определения текстуры для пули в зависимости от типа врага
    sf::IntRect BulletTextureRect;

    // if (dynamic_cast<ABaseGreenEnemy*>(this))
    // {
    //     BulletTextureRect = sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x), static_cast<int>(BULLET_SIZE.y));
    // }
    // else if (dynamic_cast<ABaseRedEnemy*>(this))
    // {
    //     BulletTextureRect = sf::IntRect(4, 24, static_cast<int>(BULLET_SIZE.x) + 10,
    //                                     static_cast<int>(BULLET_SIZE.y) + 2);
    // }
    // else if (dynamic_cast<ABaseBlueEnemy*>(this))
    // {
    //     BulletTextureRect = sf::IntRect(4, 56, static_cast<int>(BULLET_SIZE.x) + 9,
    //                                     static_cast<int>(BULLET_SIZE.y) + 2);
    // }
    if (dynamic_cast<ABossEnemy*>(this))
    {
        BulletTextureRect = sf::IntRect(3, 7, static_cast<int>(BULLET_SIZE.x) + 8,
                                        static_cast<int>(BULLET_SIZE.y) + 2);
    }
    else
    {
        BulletTextureRect = sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x),
                                        static_cast<int>(BULLET_SIZE.y));
    }

    // Выстрел из оружия, если персонаж обнаружен
    if (bIsPlayerDetected)
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? ActorSize.x : 0.f;

        // Добавляем пулю в вектор, если враг сделал выстрел
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight,
                                                  BulletTextureRect,
                                                  EBulletType::EBT_ShootAtPlayer,
                                                  sf::Vector2f(ActorCollisionRect.left + SpawnBulletOffsetX,
                                                               ActorCollisionRect.top + SpawnBulletOffsetY),
                                                  SpriteManager));
    }
}

/**
 * @brief Изменения направления врага.
 */
void AEnemy::ChangeDirection()
{
    bIsMoveRight = !bIsMoveRight;
    ActorVelocity.x = bIsMoveRight ? PawnSpeed : -PawnSpeed;
}

/**
 * @brief Вычисление позиции отрисовки врага.
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
        sf::FloatRect DetectionBounds = LineTraceDetectionArea.getGlobalBounds();
        sf::FloatRect PlayerBounds = Player.GetActorCollisionRect();

        // Проверка пересечения с препятствиями
        for (const auto& Obstacle : GameMap.GetGameMapCollisionVector())
        {
            if (DetectionBounds.intersects(Obstacle))
            {
                // Проверка, блокирует ли препятствие линию видимости
                if (Obstacle.intersects(PlayerBounds))
                {
                    bIsPlayerDetected = false;
                    break;
                }
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
        ActorVelocity.x = bIsMoveRight ? PawnSpeed * DeltaTime : -PawnSpeed * DeltaTime;
        ActorSprite.setScale(bIsMoveRight ? EnemyScale * DRAW_SCALE.x : -EnemyScale * DRAW_SCALE.x,
                             EnemyScale * DRAW_SCALE.y);

        // Обновляем анимацию спрайта врага
        ActorSprite.setTextureRect(WalkAnimation.GetCurrentFrame());
        WalkAnimation.AnimationUpdate(DeltaTime);
    }
}

/**
 * @brief Обновление позиции врага на основе текущей скорости.
 */
void AEnemy::UpdatePosition()
{
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;
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
            ChangeDirection();
        }
    }
}

/**
 * @brief Обновление движения врага.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 * @param CollisionManager Mенеджер коллизий.
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

    // Проверка столкновений врага с коллизиями карты
    CollisionManager.HandlePawnCollisionWithGameMap(ActorCollisionRect,
                                                    ActorVelocity,
                                                    bIsPawnJump,
                                                    bIsPawnJump,
                                                    this);

    // TODO: Нужна, чтобы установить положение каждого врага отдельно.
    // TODO: Позже переместиться в постоянную обработку Геймплея игры
    CalculateEnemyDrawPosition();

    // Установить положение спрайта и прямоугольника коллизии с положением врага в игре
    ActorSprite.setPosition(ActorDrawPosition);
    LineTraceDetectionArea.setPosition(ActorDrawPosition);

    // TODO: test health
    // Устанавливаем положение шкалы здоровья чуть выше врага
    UpdatePawnHealthBar(PawnCurrentHealth, PawnMaxHealth,
                        sf::Vector2f(ActorCollisionRect.left + (ActorSize.x / 2.f),
                                     ActorCollisionRect.top - 4.f));
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
