#include "SpriteManager.h"

/**
 * @brief Устанавливает размер спрайта с заданными шириной и высотой.
 *
 * @param Sprite Ссылка на объект спрайта SFML.
 * @param DesiredWidth Желаемая ширина спрайта.
 * @param DesiredHeight Желаемая высота спрайта.
 */
void ASpriteManager::SetSpriteSize(sf::Sprite& Sprite, const float DesiredWidth, const float DesiredHeight)
{
    // Получаем локальные границы спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();

    // Вычисляем масштаб по осям X и Y для достижения желаемых размеров
    const sf::Vector2f Scale = { DesiredWidth / SpriteRect.width, DesiredHeight / SpriteRect.height };

    // Устанавливаем масштаб спрайта
    Sprite.setScale(Scale);
}

/**
 * @brief Устанавливает размер формы с заданными шириной и высотой.
 *
 * @param Shape Ссылка на объект формы SFML.
 * @param DesiredWidth Желаемая ширина формы.
 * @param DesiredHeight Желаемая высота формы.
 */
void ASpriteManager::SetShapeSize(sf::Shape& Shape, const float DesiredWidth, const float DesiredHeight)
{
    // Получаем локальные границы формы
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();

    // Вычисляем масштаб по осям X и Y для достижения желаемых размеров
    const sf::Vector2f Scale = { DesiredWidth / ShapeRect.width, DesiredHeight / ShapeRect.height };

    // Устанавливаем масштаб формы
    Shape.setScale(Scale);
}

/**
 * @brief Устанавливает относительный центр происхождения спрайта.
 *
 * @param Sprite Ссылка на объект спрайта SFML.
 * @param OriginX Относительная координата X центра происхождения спрайта.
 * @param OriginY Относительная координата Y центра происхождения спрайта.
 */
void ASpriteManager::SetSpriteRelativeOrigin(sf::Sprite& Sprite, const float OriginX, const float OriginY)
{
    // Получаем локальные границы спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();

    // Вычисляем относительные координаты центра происхождения спрайта
    const sf::Vector2f Origin = { OriginX * SpriteRect.width, OriginY * SpriteRect.height };

    // Устанавливаем центр происхождения спрайта
    Sprite.setOrigin(Origin);
}

/**
 * @brief Устанавливает относительный центр происхождения формы.
 *
 * @param Shape Ссылка на объект формы SFML.
 * @param OriginX Относительная координата X центра происхождения формы.
 * @param OriginY Относительная координата Y центра происхождения формы.
 */
void ASpriteManager::SetShapeRelativeOrigin(sf::Shape& Shape, const float OriginX, const float OriginY)
{
    // Получаем локальные границы формы
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();

    // Вычисляем относительные координаты центра происхождения формы
    const sf::Vector2f Origin = { OriginX * ShapeRect.width, OriginY * ShapeRect.height };

    // Устанавливаем центр происхождения формы
    Shape.setOrigin(Origin);
}