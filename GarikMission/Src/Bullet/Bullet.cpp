#include "Bullet.h"

/**
 * @brief Конструктор для инициализации пули.
 * 
 * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
 * @param ShootAt Тип цели, на которую направлена пуля.
 * @param StartPosition Начальная позиция пули.
 * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
 */
// TODO: Испраавить EBulletType &ShootAt название аргумента конструтора
ABullet::ABullet(const bool NewVelocity, const EBulletType &ShootAt, const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager)
    : BulletDamage(BULLET_DAMAGE)
    , BulletType(ShootAt)
    , SpriteManagerPtr(new ASpriteManager)
{
    InitBullet(NewVelocity, StartPosition, SpriteManager);
}

/**
 * @brief Деструктор пули.
 * 
 * Освобождает память, выделенную под текстуру пули.
 */
ABullet::~ABullet()
{
    delete SpriteManagerPtr;
}

/**
 * @brief Инициализирует текструру и создаёт спрайт для пули.
 * 
 * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
 * @param StartPosition Начальная позиция пули.
 * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
 */
void ABullet::InitBullet(const bool NewVelocity, const sf::Vector2f& StartPosition,  ASpriteManager &SpriteManager)
{
    // Установить направление, откуда начать движение
    ActorVelocity.x = NewVelocity ? BULLET_SPEED : -BULLET_SPEED;

    // Устанавливаем размер коллизии для пули
    ActorCollisionRect = {StartPosition.x, StartPosition.y, BULLET_SIZE.x * DRAW_SCALE.x, BULLET_SIZE.y * DRAW_SCALE.y};

    // Инициализировать текстуру для пули и создать спрайт для неё
    InitActorTexture(ASSETS_PATH + "MainTiles/Bullets.png",
                    sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x), static_cast<int>(BULLET_SIZE.y)),
                    {BULLET_SIZE.x * (DRAW_SCALE.x - 3),BULLET_SIZE.y * (DRAW_SCALE.y - 3)},
                    {0.5f, 0.5f}, *SpriteManagerPtr);

    // Задать стартовую позицию пули
    ActorSprite.setPosition(StartPosition.x, StartPosition.y);
}

/**
 * @brief Обновление положения пули.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 */
void ABullet::UpdateBulletPosition(float DeltaTime)
{
    // Обновляем положение пули с учетом скорости и времени
    ActorCollisionRect.left += ActorVelocity.x * DeltaTime;
    ActorCollisionRect.top -= ActorVelocity.y * DeltaTime;

    // Устанавливаем положение спрайта с коллизией
    ActorSprite.setPosition(ActorCollisionRect.left, ActorCollisionRect.top);
}

/**
 * @brief Получение урона от пули.
 * 
 * @return Урон, наносимый пулей.
 */
int ABullet::GetBulletDamage() const
{
    return BulletDamage;
}

/**
 * @brief Получение типа пули.
 * 
 * @return Тип пули (стрелять в игрока или во врага).
 */
EBulletType ABullet::GetBulletType() const
{
    return BulletType;
}

/**
 * @brief Отрисовка пули на экране.
 * 
 * @param Window Окно для отрисовки.
 */
void ABullet::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}