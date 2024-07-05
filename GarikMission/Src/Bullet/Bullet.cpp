#include "Bullet.h"

/**
 * @brief Конструктор для инициализации пули.
 * 
 * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
 * @param BulletTextureRect Прямоугольник текстуры пули.
 * @param ShootAt Тип цели, на которую направлена пуля.
 * @param StartPosition Начальная позиция пули.
 * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
 */
ABullet::ABullet(const bool NewVelocity, const sf::IntRect& BulletTextureRect, const EBulletType& ShootAt,
                 const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager)
    : bIsDestroyed(false)
      , BulletDamage(BULLET_DAMAGE)
      , BulletType(ShootAt)
{
    ABullet::InitBullet(NewVelocity, BulletTextureRect, StartPosition, SpriteManager);
}

/**
 * @brief Инициализирует текстуру и создаёт спрайт для пули.
 * 
 * @param NewVelocity Флаг, определяющий, имеет ли пуля новую скорость.
 * @param BulletTextureRect Прямоугольник текстуры пули.
 * @param StartPosition Начальная позиция пули.
 * @param SpriteManager Ссылка на менеджер спрайтов для отрисовки.
 */
void ABullet::InitBullet(const bool NewVelocity, const sf::IntRect& BulletTextureRect,
                         const sf::Vector2f& StartPosition, ASpriteManager& SpriteManager)
{
    // Инициализация переменных для текстуры пули
    const std::string BulletTexturePath = ASSETS_PATH + "MainTiles/Bullets.png";
    const sf::Vector2f BulletSize = {
        static_cast<float>(BulletTextureRect.getSize().x),
        static_cast<float>(BulletTextureRect.getSize().y)
    };
    const sf::Vector2f BulletOrigin = {0.5f, 0.5f};

    // Установить направление, откуда начать движение
    ActorVelocity.x = NewVelocity ? BULLET_SPEED : -BULLET_SPEED;

    // Устанавливаем размер коллизии для пули
    ActorCollisionRect = {StartPosition.x, StartPosition.y, BULLET_SIZE.x, BULLET_SIZE.y};

    // Инициализировать текстуру для пули и создать спрайт для неё
    InitActorTexture(BulletTexturePath, BulletTextureRect, BulletSize, BulletOrigin, SpriteManager);

    // Задать стартовую позицию пули
    ActorSprite.setPosition(StartPosition);

    sf::Vector2f ScaleBullet = NewVelocity ? sf::Vector2f(0.7f, 0.7f) : sf::Vector2f(-0.7f, 0.7f);
    ActorSprite.setScale(ScaleBullet);

    // TODO: Для тестирования разрушения анимации пули
    // AnimBulletDestroy.AnimTexture.loadFromFile(BulletTexturePath);
    // AnimBulletDestroy.FrameSpeed = 5.f;
    // AnimBulletDestroy.FrameRect.emplace_back(BulletTextureRect);
    // AnimBulletDestroy.FrameRect.emplace_back(22, 70, static_cast<int>(BULLET_SIZE.x) + 5,
    //                                          static_cast<int>(BULLET_SIZE.y) + 5);
    // AnimBulletDestroy.FrameRect.emplace_back(37, 71, static_cast<int>(BULLET_SIZE.x) + 5,
    //                                          static_cast<int>(BULLET_SIZE.y) + 5);
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

    // TODO: Для тестирования разрушения анимации пули
    // if (bIsDestroyed)
    // {
    //     AnimBulletDestroy.AnimationUpdate(DeltaTime);
    //     ActorSprite.setTextureRect(AnimBulletDestroy.GetCurrentFrame());
    //
    //     if (AnimBulletDestroy.GetCurrentFrameIndex() == 0)
    //     {
    //         // Удаляем пулю после завершения анимации
    //         delete this;
    //     }
    // }
    // else
    // {
    //     // Обновляем положение пули с учетом скорости и времени
    //     ActorCollisionRect.left += ActorVelocity.x * DeltaTime;
    //     ActorCollisionRect.top -= ActorVelocity.y * DeltaTime;
    //     ActorSprite.setPosition(ActorCollisionRect.left, ActorCollisionRect.top);
    // }
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
