#pragma once
#include "../Constants.h"

class IRendererSprite
{
public:
    
    IRendererSprite &operator= (const IRendererSprite&) = delete;
    IRendererSprite &operator=(IRendererSprite&&) = delete;
    
    virtual ~IRendererSprite() = default;
    
    virtual void SetSpriteSize(sf::Sprite& Sprite, float DesiredWidth, float DesiredHeight) = 0;
    virtual void SetShapeSize(sf::Shape& Shape, float DesiredWidth, float DesiredHeight) = 0;

    virtual void SetSpriteRelativeOrigin(sf::Sprite& Sprite, float OriginX, float OriginY) = 0;
    virtual void SetShapeRelativeOrigin(sf::Shape& Shape, float OriginX, float OriginY) = 0;
};