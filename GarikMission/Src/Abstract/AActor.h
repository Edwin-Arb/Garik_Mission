#pragma once
#include "../Constants.h"
#include "../Manager/SpriteManager.h"

class AActor
{
public:
    AActor();
    virtual ~AActor() = default;

    /**
     * @brief Инициализирует текстуру объекта.
     * 
     * @param TexturePath Путь к текстуре.
     * @param TextureRect Прямоугольник, ограничивающий текстуру.
     * @param Size Размер объекта.
     * @param Scale Масштабирование объекта.
     * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
     */
    virtual void InitActorTexture(const std::string& TexturePath,
                                  const sf::IntRect& TextureRect,
                                  const sf::Vector2f& Size,
                                  const sf::Vector2f& Scale,
                                  ASpriteManager& SpriteManager);

    /**
     * @brief Инициализирует текстуру объекта.
     * 
     * @param TextureRect Прямоугольник, ограничивающий текстуру.
     * @param Size Размер объекта.
     * @param Scale Масштабирование объекта.
     * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
     */
    virtual void InitActorTexture(const sf::IntRect& TextureRect,
                                  const sf::Vector2f& Size,
                                  const sf::Vector2f& Scale,
                                  ASpriteManager& SpriteManager);

private:
    /**
     * @brief Инициализирует спрайт объекта.
     * 
     * @param TexturePtr Указатель на текстуру.
     * @param TextureRect Прямоугольник, ограничивающий текстуру.
     * @param Size Размер объекта.
     * @param Scale Масштабирование объекта.
     * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
     */
    virtual void InitActorSprite(const std::unique_ptr<sf::Texture>& TexturePtr,
                                 const sf::IntRect& TextureRect,
                                 const sf::Vector2f& Size,
                                 const sf::Vector2f& Scale,
                                 ASpriteManager& SpriteManager);

public:
    /**
     * @brief Устанавливает новую скорость объекта.
     * 
     * @param NewVelocity Новая скорость объекта.
     */
    void SetActorVelocity(const sf::Vector2f& NewVelocity);

    /**
     * @brief Устанавливает текущую позицию объекта.
     *
     * @param NewPosition Новая позиция объекта.
     */
    void SetActorPosition(const sf::Vector2f& NewPosition);
    
    /**
     * @brief Устанавливает прямоугольник объекта.
     *
     * @param NewRect Новый прямоугольник, ограничивающий объект.
     */
    void SetActorCollisionRect(const sf::FloatRect& NewRect);

    /**
     * @brief Получает по ссылки текущую скорость объекта.
     *
     * @return Текущая скорость объекта.
     */
    sf::Vector2f &GetActorVelocity();

    /**
     * @brief Получает по ссылки текущую позицию объекта.
     *
     * @return Текущая позиция объекта.
     */
    sf::Vector2f &GetActorPosition();

    /**
     * @brief Получает по ссылки прямоугольник, ограничивающий объект.
     *
     * @return Прямоугольник, ограничивающий объект.
     */
    sf::FloatRect &GetActorCollisionRect();

    /**
     * @brief Отрисовывает объект на экране. Будет переопределён в классах-наследниках.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    virtual void DrawActor(sf::RenderWindow& Window) = 0;

protected:
    sf::Vector2f ActorVelocity;                       // Вектор скорости.
    sf::FloatRect ActorCollisionRect;                 // Прямоугольник коллизии.
    sf::Vector2f ActorDrawPosition;                   // Позиция отрисовки.

    std::unique_ptr<sf::Texture> ActorTexturePtr;     // Указатель на текстуру.
    sf::Sprite ActorSprite;                           // Спрайт для установки текстуры.
};