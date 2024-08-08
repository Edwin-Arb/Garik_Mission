#include "BossEnemy.h"

/**
 * @brief Конструктор для инициализации босса.
 * 
 * @param MaxPatrolDistance Максимальная дистанция патрулирования босса.
 * @param StartPosition Начальная позиция босса.
 */
ABossEnemy::ABossEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
      , BossSpecialAttackCooldown(3.f)
      , CurrentSpecialAttackCooldown(0.f)
{
    // Установка параметров босса
    PawnMaxHealth = ENEMY_MAX_HEALTH * 2.f;
    PawnCurrentHealth = PawnMaxHealth;
    PawnSpeed = ENEMY_SPEED;

    EnemyScale = {1.5f};

    ActorSize = {15.f * DRAW_SCALE.x, 22.f * DRAW_SCALE.y};

    ActorCollisionRect = {
        StartPosition.x, StartPosition.y,
        ActorSize.x, ActorSize.y
    };

    EnemyHealthBarSize = sf::Vector2f(17.f, 3.f);
}

/**
 * @brief Инициализирует босса текстурами и анимациями.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void ABossEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // Инициализация базового класса
    AEnemy::InitEnemy(SpriteManager);

    // Установка текстуры и анимации для босса
    const sf::IntRect BossRectTexture = sf::IntRect(48, 33, static_cast<int>(ENEMY_SIZE.x),
                                                    static_cast<int>(ENEMY_SIZE.y));

    InitActorTexture(EnemyTexturePath, BossRectTexture, ActorSize, EnemyOrigin, SpriteManager);

    RectCollisionTest.setSize(ActorSize);

    WalkAnimation.AnimTexture.loadFromFile(EnemyTexturePath);
    WalkAnimation.FrameSpeed = 5.f;
    WalkAnimation.FrameRect.emplace_back(BossRectTexture);
    WalkAnimation.FrameRect.emplace_back(32, 33, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));
}

/**
 * @brief Выполняет выстрелы босса.
 * 
 * @param SpawnBulletOffsetY Смещение по оси Y для спавна пули.
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void ABossEnemy::EnemyShoot(const float SpawnBulletOffsetY, std::vector<ABullet*>& BulletsVectorPtr,
                            ASpriteManager& SpriteManager)
{
    AEnemy::EnemyShoot(15.f, BulletsVectorPtr, SpriteManager);

    // Реализация специальной атаки босса
    // if (CurrentSpecialAttackCooldown <= 0.f)
    // {
    //     SpecialAttack(BulletsVectorPtr, SpriteManager);
    //     CurrentSpecialAttackCooldown = BossSpecialAttackCooldown;
    // }
}

/**
 * @brief Выполняет специальную атаку босса.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void ABossEnemy::SpecialAttack(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const
{
    // TODO: Пример реализации специальной атаки босса
    // Например, стрельба веером из нескольких пуль
    // for (int i = -2; i <= 2; ++i)
    // {
    //     float OffsetX = (bIsMoveRight ? 10.f : 0.f) + i * 5.f;
    //     BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight,
    //                                               EBulletType::EBT_ShootAtPlayer,
    //                                               sf::Vector2f(ActorCollisionRect.left + OffsetX,
    //                                                            ActorCollisionRect.top + 9.f),
    //                                               SpriteManager));
    // }
}

/**
 * @brief Обновляет движение босса.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект игрока.
 * @param GameMap Ссылка на объект игровой карты.
 * @param CollisionManager Менеджер коллизий.
 */
void ABossEnemy::UpdateEnemyMove(float DeltaTime, APlayer& Player,
                                 const AGameMap& GameMap,
                                 const ACollisionManager& CollisionManager)
{
    // Обновление движения базового класса
    AEnemy::UpdateEnemyMove(DeltaTime, Player, GameMap, CollisionManager);

    // TODO: Test collision
    // RectCollisionTest.setPosition(ActorCollisionRect.left, ActorCollisionRect.top);

    // Обновление времени до следующей специальной атаки
    if (CurrentSpecialAttackCooldown > 0.f)
    {
        CurrentSpecialAttackCooldown -= DeltaTime;
    }
}

void ABossEnemy::OnDeath(std::vector<AKeyObject*>& KeysVector, ASpriteManager &SpriteManager, const sf::Vector2f& BossPosition)
{
    // Создаем ключ на позиции босса
    AKeyObject* NewKey = new AKeyObject(BossPosition);
    NewKey->InitKeys(SpriteManager);
    ++NewKey->KeysCountFound;
    KeysVector.emplace_back(NewKey);
}
