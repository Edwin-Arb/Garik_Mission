#pragma once
#include "../Constants.h"


class ASpriteManager
{
public:
    
    void SetSpriteSize(sf::Sprite& Sprite, float DesiredWidth, float DesiredHeight);
    void SetShapeSize(sf::Shape& Shape, float DesiredWidth, float DesiredHeight);
    void SetSpriteRelativeOrigin(sf::Sprite& Sprite, float OriginX, float OriginY);
    void SetShapeRelativeOrigin(sf::Shape& Shape, float OriginX, float OriginY);
};
