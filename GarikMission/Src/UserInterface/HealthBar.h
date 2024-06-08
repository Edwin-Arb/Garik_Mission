#pragma once
#include "../Constants.h"
#include "../Manager/SpriteManager.h"

class AHealthBar
{
public:
    AHealthBar() = default;
    ~AHealthBar() = default;
    
    void InitHealthBar(float Width, float Height, const sf::Color& FillColor, const sf::Color& BackgroundColor, ASpriteManager& SpriteManager);
    void SetPosition(const sf::Vector2f& Position);
    void Update(float CurrentHealth, float MaxHealth);
    void Draw(sf::RenderWindow& Window) const;

private:
    
    sf::RectangleShape BackgroundBarRect;
    sf::RectangleShape HealthBarRect;
};
