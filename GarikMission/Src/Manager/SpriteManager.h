#pragma once
#include "../Renderer/IRendererSprite.h"


class ASpriteManager : public IRendererSprite
{
public:
    
    void SetSpriteSize(sf::Sprite& Sprite, float DesiredWidth, float DesiredHeight) override;
    void SetShapeSize(sf::Shape& Shape, float DesiredWidth, float DesiredHeight) override;

    void SetSpriteRelativeOrigin(sf::Sprite& Sprite, float OriginX, float OriginY) override;
    void SetShapeRelativeOrigin(sf::Shape& Shape, float OriginX, float OriginY) override;
};
