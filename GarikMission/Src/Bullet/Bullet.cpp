#include "Bullet.h"

/**
 * @brief Конструктор пули.
 * 
 * @param NewVelocity Указывает направление движения пули (true - вправо, false - влево).
 * @param ShootAt Тип пули (стрелять в игрока или во врага).
 * @param StartPosition Начальная позиция пули.
 * @param RendererSprite Менеджер спрайтов для обработки спрайтов пули.
 */

// TODO: Испраавить EBulletType &ShootAt название аргумента конструтора
ABullet::ABullet(const bool NewVelocity, const EBulletType &ShootAt, const sf::Vector2f& StartPosition, ASpriteManager& RendererSprite)
    : BulletDamage(BULLET_DAMAGE)
    , BulletType(ShootAt)
    , BulletTexturePtr(new sf::Texture)
{
    // Установить направление, откуда начать движение
    BulletVelocity.x = NewVelocity ? BULLET_SPEED : -BULLET_SPEED;

    // Устанавливаем размер коллизии для пули
    BulletRect = {StartPosition.x, StartPosition.y, BULLET_SIZE.x * DRAW_SCALE.x, BULLET_SIZE.y * DRAW_SCALE.y};

    // Получить файл текстуры
    assert(BulletTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/Bullets.png"));

    // Установить спрайт для пули
    BulletSprite.setTexture(*BulletTexturePtr);
    BulletSprite.setTextureRect(sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x),
                                            static_cast<int>(BULLET_SIZE.y)));

    // Установить масштаб пули
    RendererSprite.SetSpriteSize(BulletSprite, BULLET_SIZE.x * (DRAW_SCALE.x - 3),
                                 BULLET_SIZE.y * (DRAW_SCALE.y - 3));

    // Установить центр спрайта
    RendererSprite.SetSpriteRelativeOrigin(BulletSprite, 0.5f, 0.5f);

    // Задать стартовую позицию пули
    BulletSprite.setPosition(StartPosition.x, StartPosition.y);
}

/**
 * @brief Деструктор пули.
 * 
 * Освобождает память, выделенную под текстуру пули.
 */
ABullet::~ABullet()
{
    delete BulletTexturePtr; // Освобождаем память, выделенную под текстуру пули
}

/**
 * @brief Проверка коллизии пули с прямоугольником.
 * 
 * @param TargetRect Прямоугольник цели.
 * 
 * @return true, если произошла коллизия, иначе false.
 */
bool ABullet::CheckCollision(const sf::FloatRect& TargetRect) const
{
    return BulletRect.intersects(TargetRect);
}

/**
 * @brief Обновление положения пули.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 */
void ABullet::UpdateBulletPosition(float DeltaTime)
{
    // Обновляем положение пули с учетом скорости и времени
    BulletRect.left += BulletVelocity.x * DeltaTime;
    BulletRect.top -= BulletVelocity.y * DeltaTime;

    // Устанавливаем положение спрайта с коллизией
    BulletSprite.setPosition(BulletRect.left, BulletRect.top);
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
 * @brief Получение прямоугольника коллизии пули.
 * 
 * @return Прямоугольник коллизии пули.
 */
sf::FloatRect ABullet::GetBulletCollider() const
{
    return BulletRect;
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
void ABullet::DrawBullet(sf::RenderWindow& Window) const
{
    Window.draw(BulletSprite);
}