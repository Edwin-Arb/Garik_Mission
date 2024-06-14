#include "SpriteManager.h"

void ASpriteManager::SetSpriteSize(sf::Sprite& Sprite, const float DesiredWidth, const float DesiredHeight)
{
    // Получаем локальные границы спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();

    // Вычисляем масштаб по осям X и Y для достижения желаемых размеров
    const sf::Vector2f Scale = { DesiredWidth / SpriteRect.width, DesiredHeight / SpriteRect.height };

    // Устанавливаем масштаб спрайта
    Sprite.setScale(Scale);
}

void ASpriteManager::SetShapeSize(sf::Shape& Shape, const float DesiredWidth, const float DesiredHeight)
{
    // Получаем локальные границы формы
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();

    // Вычисляем масштаб по осям X и Y для достижения желаемых размеров
    const sf::Vector2f Scale = { DesiredWidth / ShapeRect.width, DesiredHeight / ShapeRect.height };

    // Устанавливаем масштаб формы
    Shape.setScale(Scale);
}

void ASpriteManager::SetSpriteRelativeOrigin(sf::Sprite& Sprite, const float OriginX, const float OriginY)
{
    // Получаем локальные границы спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();

    // Вычисляем относительные координаты центра происхождения спрайта
    const sf::Vector2f Origin = { OriginX * SpriteRect.width, OriginY * SpriteRect.height };

    // Устанавливаем центр происхождения спрайта
    Sprite.setOrigin(Origin);
}

void ASpriteManager::SetShapeRelativeOrigin(sf::Shape& Shape, const float OriginX, const float OriginY)
{
    // Получаем локальные границы формы
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();

    // Вычисляем относительные координаты центра происхождения формы
    const sf::Vector2f Origin = { OriginX * ShapeRect.width, OriginY * ShapeRect.height };

    // Устанавливаем центр происхождения формы
    Shape.setOrigin(Origin);
}
