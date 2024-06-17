#include "AActor.h"
#include <cassert>

AActor::AActor()

{
}

/**
 * @brief Инициализирует текстуру объекта.
 * 
 * @param TexturePath Путь к текстуре.
 * @param TextureRect Прямоугольник, ограничивающий текстуру.
 * @param Size Размер объекта.
 * @param Scale Масштабирование объекта.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void AActor::InitActorTexture(const std::string& TexturePath, const sf::IntRect& TextureRect,
                              const sf::Vector2f& Size, const sf::Vector2f& Scale,
                              ASpriteManager& SpriteManager)
{
    ActorTexturePtr = std::make_unique<sf::Texture>();

    // Подгрузить текстуру из файла
    assert(ActorTexturePtr->loadFromFile(TexturePath));

    InitActorSprite(ActorTexturePtr, TextureRect, Size, Scale, SpriteManager);
}

void AActor::InitActorTexture(const sf::IntRect& TextureRect,
                              const sf::Vector2f& Size, const sf::Vector2f& Scale,
                              ASpriteManager& SpriteManager)
{
    ActorTexturePtr = std::make_unique<sf::Texture>();

    // Подгрузить текстуру из файла
    // assert(ActorTexturePtr->loadFromFile(TexturePath));

    InitActorSprite(ActorTexturePtr, TextureRect, Size, Scale, SpriteManager);
}

/**
 * @brief Инициализирует спрайт объекта.
 * 
 * @param TexturePtr Указатель на текстуру.
 * @param TextureRect Прямоугольник, ограничивающий текстуру.
 * @param Size Размер объекта.
 * @param Scale Масштабирование объекта.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void AActor::InitActorSprite(const std::unique_ptr<sf::Texture>& TexturePtr, const sf::IntRect& TextureRect,
                             const sf::Vector2f& Size, const sf::Vector2f& Scale, ASpriteManager& SpriteManager)
{
    // Создать спрайт объекта и установить его текстуру и прямоугольник
    ActorSprite.setTexture(*TexturePtr);
    ActorSprite.setTextureRect(TextureRect);

    // Установить масштаб объекта
    SpriteManager.SetSpriteSize(ActorSprite, Size.x, Size.y);

    // Установить центр спрайта и коллизии
    SpriteManager.SetSpriteRelativeOrigin(ActorSprite, Scale.x, Scale.y);
}

/**
 * @brief Устанавливает новую скорость объекта.
 * 
 * @param NewVelocity Новая скорость объекта.
 */
void AActor::SetActorVelocity(const sf::Vector2f& NewVelocity)
{
    ActorVelocity = NewVelocity;
}

/**
 * @brief Устанавливает текущую позицию объекта.
 *
 * @param NewPosition Новая позиция объекта.
 */
void AActor::SetActorPosition(const sf::Vector2f& NewPosition)
{
    ActorDrawPosition = NewPosition;
}

/**
 * @brief Устанавливает прямоугольник объекта.
 *
 * @param NewRect Новый прямоугольник, ограничивающий объект.
 */
void AActor::SetActorCollisionRect(const sf::FloatRect& NewRect)
{
    ActorCollisionRect = NewRect;
}

/**
 * @brief Получает по ссылки текущую скорость объекта.
 *
 * @return Текущая скорость объекта.
 */
sf::Vector2f& AActor::GetActorVelocity()
{
    return ActorVelocity;
}

/**
 * @brief Получает по ссылки текущую позицию объекта.
 *
 * @return Текущая позиция объекта.
 */
sf::Vector2f& AActor::GetActorPosition()
{
    return ActorDrawPosition;
}

/**
 * @brief Получает по ссылки прямоугольник, ограничивающий объект.
 *
 * @return Прямоугольник, ограничивающий объект.
 */
sf::FloatRect& AActor::GetActorCollisionRect()
{
    return ActorCollisionRect;
}
