#include "BossEnemy.h"


/**
 * @brief Конструктор босса.
 * 
 * @param MaxPatrolDistance Максимальная дистанция патрулирования.
 * @param StartPosition Начальная позиция босса.
 */
ABossEnemy::ABossEnemy(float MaxPatrolDistance, const sf::Vector2f& StartPosition)
    : AEnemy(MaxPatrolDistance, StartPosition)
      , BossSpecialAttackCooldown(3.f)
      , CurrentSpecialAttackCooldown(0.f)
{
    // Увеличение параметров босса
    PawnMaxHealth = ENEMY_MAX_HEALTH * 2.f;
    PawnCurrentHealth = PawnMaxHealth;
    PawnSpeed = ENEMY_SPEED;

    EnemyScale = {1.5f};

    ActorSize = {15.f * DRAW_SCALE.x, 22.f * DRAW_SCALE.y};

    ActorCollisionRect = {
        StartPosition.x, StartPosition.y,
        ActorSize.x, ActorSize.y
    };
}

/**
 * @brief Инициализация босса.
 * 
 * @param SpriteManager Менеджер спрайтов для загрузки ресурсов.
 */
void ABossEnemy::InitEnemy(ASpriteManager& SpriteManager)
{
    // Используем базовый метод для инициализации
    AEnemy::InitEnemy(SpriteManager);

    // Установка уникальной текстуры и анимаций для босса
    const std::string BossTexturePath = ASSETS_PATH + "MainTiles/Enemy.png";
    const sf::IntRect BossRectTexture = sf::IntRect(48, 33, static_cast<int>(ENEMY_SIZE.x),
                                                    static_cast<int>(ENEMY_SIZE.y));
    //const sf::Vector2f BossSize = {ActorSize.x * DRAW_SCALE.x, ActorSize.y * DRAW_SCALE.y};
    const sf::Vector2f BossOrigin = {0.5f, 0.5f};

    // Инициализировать текстуру для босса и создание спрайта для него.
    InitActorTexture(BossTexturePath, BossRectTexture, ActorSize, BossOrigin, SpriteManager);
    //ActorSprite.setScale(20.f, 20.f);

    // BossTexture.loadFromFile(BossTexturePath);
    // BossSprite.setTexture(BossTexture);
    // BossSprite.setOrigin(BossOrigin);
    // BossSprite.setTextureRect(BossRectTexture);
    // SpriteManager.SetSpriteSize(BossSprite, ActorSize);


    RectCollisionTest.setSize(ActorSize);


    //WalkAnimation.AnimTexture.loadFromFile(BossTexturePath);
    //WalkAnimation.FrameSpeed = 5.f;
    //WalkAnimation.FrameRect.emplace_back(BossRectTexture);
    //WalkAnimation.FrameRect.emplace_back(32, 33, static_cast<int>(ENEMY_SIZE.x), static_cast<int>(ENEMY_SIZE.y));
    //
    //SpriteManager.SetShapeRelativeOrigin(LineTraceDetectionArea, 0.5f, 0.5f);
    //LineTraceDetectionArea.setFillColor(sf::Color(155, 0, 0, 128));
    //InitPawnHealthBar(sf::Vector2f(20.f, 4.f), sf::Vector2f(0.4f, 0.4f), sf::Color::Green, sf::Color::Red, SpriteManager);
}

/**
 * @brief Босс открывает огонь в сторону персонажа.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void ABossEnemy::EnemyShoot(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager)
{
    AEnemy::EnemyShoot(BulletsVectorPtr, SpriteManager);
    // Реализация специальной атаки босса
    if (CurrentSpecialAttackCooldown <= 0.f)
    {
        SpecialAttack(BulletsVectorPtr, SpriteManager);
        CurrentSpecialAttackCooldown = BossSpecialAttackCooldown;
    }
}

// void ABossEnemy::CalculateEnemyDrawPosition(const sf::FloatRect& EnemyRectRef, const sf::Vector2f& EnemySize,
//     const sf::Vector2f& DrawScale)
// {
//     AEnemy::CalculateEnemyDrawPosition(EnemyRectRef, EnemySize, DrawScale);
// }

/**
 * @brief Специальная атака босса.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль, в который добавляются выстрелы.
 * @param SpriteManager Менеджер спрайтов для управления спрайтами.
 */
void ABossEnemy::SpecialAttack(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const
{
    // TODO: Пример реализации специальной атаки: стрельба веером из нескольких пуль
    for (int i = -2; i <= 2; ++i)
    {
        float OffsetX = (bIsMoveRight ? 10.f : 0.f) + i * 5.f;
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, EBulletType::EBT_ShootAtPlayer,
                                                  sf::Vector2f(ActorCollisionRect.left + OffsetX,
                                                               ActorCollisionRect.top + 9.f),
                                                  SpriteManager));
    }
}

/**
 * @brief Обновление движения босса.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 * @param CollisionManager Менеджер коллизий.
 */
void ABossEnemy::UpdateEnemyMove(float DeltaTime, APlayer& Player,
                                 const AGameMap& GameMap,
                                 const ACollisionManager& CollisionManager)
{
    // Используем базовый метод для обновления движения
    AEnemy::UpdateEnemyMove(DeltaTime, Player, GameMap, CollisionManager);

    // TODO: Test collision
    //RectCollisionTest.setPosition(ActorCollisionRect.left, ActorCollisionRect.top);
    //BossSprite.setPosition(ActorCollisionRect.left, ActorCollisionRect.top);

    // Обновление кулдауна специальной атаки
    if (CurrentSpecialAttackCooldown > 0.f)
    {
        CurrentSpecialAttackCooldown -= DeltaTime;
    }
}

void ABossEnemy::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(RectCollisionTest);
    Window.draw(BossSprite);

    AEnemy::DrawActor(Window);
}
