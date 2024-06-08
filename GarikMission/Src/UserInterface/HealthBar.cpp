#include "HealthBar.h"



void AHealthBar::InitHealthBar(float Width, float Height, const sf::Color& FillColor, const sf::Color& BackgroundColor,
                               ASpriteManager& SpriteManager)
{
    BackgroundBarRect.setSize(sf::Vector2f(Width, Height));
    BackgroundBarRect.setFillColor(BackgroundColor);

    HealthBarRect.setSize(sf::Vector2f(Width, Height));
    HealthBarRect.setFillColor(FillColor);

    SpriteManager.SetShapeRelativeOrigin(BackgroundBarRect, 0.5f, 0.5f);
    SpriteManager.SetShapeRelativeOrigin(HealthBarRect, 0.5f, 0.5f);
}

void AHealthBar::SetPosition(const sf::Vector2f& position)
{
    BackgroundBarRect.setPosition(position);
    HealthBarRect.setPosition(position);
}

void AHealthBar::Update(float CurrentHealth, float MaxHealth)
{
    float HealthPercentage = CurrentHealth / MaxHealth;
    sf::Vector2f Size = BackgroundBarRect.getSize();
    HealthBarRect.setSize(sf::Vector2f(Size.x * HealthPercentage, Size.y));
}

void AHealthBar::Draw(sf::RenderWindow& Window) const
{    
    Window.draw(BackgroundBarRect);
    Window.draw(HealthBarRect);
}
