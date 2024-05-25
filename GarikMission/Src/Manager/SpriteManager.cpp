#include "SpriteManager.h"


void ASpriteManager::SetSpriteSize(sf::Sprite& Sprite, const float DesiredWidth, const float DesiredHeight)
{// Установить размер спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();
    const sf::Vector2f Scale = { DesiredWidth / SpriteRect.width, DesiredHeight / SpriteRect.height };
    Sprite.setScale(Scale);
}

void ASpriteManager::SetShapeSize(sf::Shape& Shape, const float DesiredWidth, const float DesiredHeight)
{// Установить размер формы(shape)
    const sf::FloatRect SpriteRect = Shape.getLocalBounds();
    const sf::Vector2f Scale = { DesiredWidth / SpriteRect.width, DesiredHeight / SpriteRect.height };
    Shape.setScale(Scale);
}

void ASpriteManager::SetSpriteRelativeOrigin(sf::Sprite& Sprite, const float OriginX, const float OriginY)
{// Установить центр координа для спрайта(тексты объекта)
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();
    Sprite.setOrigin(OriginX * SpriteRect.width, OriginY * SpriteRect.height);
}

void ASpriteManager::SetShapeRelativeOrigin(sf::Shape& Shape, const float OriginX, const float OriginY)
{// Установить центр координа для формы(коллизии)
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();
    Shape.setOrigin(OriginX * ShapeRect.width, OriginY * ShapeRect.height);
}
