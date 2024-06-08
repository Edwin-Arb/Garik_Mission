#include "Test.h"

RoundedRectangle::RoundedRectangle(float Width, float Height, float Radius, const sf::Color& Color)
{
    CreateCenter(Width, Height, Radius, Color);
    CreateCorners(Width, Height, Radius, Color);
    CreateSides(Width, Height, Radius, Color);
}

// Установка позиции закругленного прямоугольника
void RoundedRectangle::SetPosition(const sf::Vector2f& NewPosition)
{    
    // Установка позиции центрального прямоугольника
    Center.setPosition(NewPosition.x + Center.getPosition().x, NewPosition.y + Center.getPosition().y);

    // Установка позиций углов
    for (int i = 0; i < 4; ++i)
    {
        Corners[i].setPosition(NewPosition.x + Corners[i].getPosition().x, NewPosition.y + Corners[i].getPosition().y);
    }

    // Установка позиций сторон
    Top.setPosition(NewPosition.x + Top.getPosition().x, NewPosition.y + Top.getPosition().y);
    Bottom.setPosition(NewPosition.x + Bottom.getPosition().x, NewPosition.y + Bottom.getPosition().y);
    Left.setPosition(NewPosition.x + Left.getPosition().x, NewPosition.y + Left.getPosition().y);
    Right.setPosition(NewPosition.x + Right.getPosition().x, NewPosition.y + Right.getPosition().y);
}

// Создание центрального прямоугольника
void RoundedRectangle::CreateCenter(float Width, float Height, float Radius, const sf::Color& Color)
{
    // Установка размера
    Center.setSize(sf::Vector2f(Width - 2 * Radius, Height - 2 * Radius));

    // Установка цвета
    Center.setFillColor(Color);

    // Установка позиции
    Center.setPosition(Radius, Radius);
}

// Создание углов
void RoundedRectangle::CreateCorners(float Width, float Height, float Radius, const sf::Color& Color)
{
    for (int i = 0; i < 4; ++i)
    {
        Corners[i].setRadius(Radius);  // Установка радиуса
        Corners[i].setFillColor(Color);  // Установка цвета
    }

    // Установка позиций углов
    Corners[0].setPosition(0, 0);  // Верхний левый
    Corners[1].setPosition(Width - Radius * 2, 0);  // Верхний правый
    Corners[2].setPosition(0, Height - Radius * 2);  // Нижний левый
    Corners[3].setPosition(Width - Radius * 2, Height - Radius * 2);  // Нижний правый
}

// Создание сторон
void RoundedRectangle::CreateSides(float Width, float Height, float Radius, const sf::Color& Color)
{
    // Верхняя сторона
    Top.setSize(sf::Vector2f(Width - 2 * Radius, Radius));
    Top.setFillColor(Color);
    Top.setPosition(Radius, 0);

    // Нижняя сторона
    Bottom.setSize(sf::Vector2f(Width - 2 * Radius, Radius));
    Bottom.setFillColor(Color);
    Bottom.setPosition(Radius, Height - Radius);

    // Левая сторона
    Left.setSize(sf::Vector2f(Radius, Height - 2 * Radius));
    Left.setFillColor(Color);
    Left.setPosition(0, Radius);

    // Правая сторона
    Right.setSize(sf::Vector2f(Radius, Height - 2 * Radius));
    Right.setFillColor(Color);
    Right.setPosition(Width - Radius, Radius);
}

// Отрисовка закругленного прямоугольника
void RoundedRectangle::Draw(sf::RenderWindow& Window) const
{    
    Window.draw(Center);  // Отрисовка центрального прямоугольника

    // Отрисовка углов
    for (const auto& Corner : Corners)
    {
        Window.draw(Corner);
    }

    // Отрисовка сторон
    Window.draw(Top);
    Window.draw(Bottom);
    Window.draw(Left);
    Window.draw(Right);
}