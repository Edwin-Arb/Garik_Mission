#pragma once
#include "../Constants.h"

class AActor
{
public:
    virtual ~AActor() = default;

    virtual void DrawActor(sf::RenderWindow& Window) = 0;

protected:
    sf::Vector2f Velocity;
    sf::FloatRect CollisionRect;
};
