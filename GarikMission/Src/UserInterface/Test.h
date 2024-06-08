#pragma once
#include "../Constants.h"

class RoundedRectangle
{
public:
    // Конструктор для создания закругленного прямоугольника
    RoundedRectangle(float Width, float Height, float Radius, const sf::Color& Color);\
    
    void SetPosition(const sf::Vector2f& NewPosition);

private:
    // Создание центрального прямоугольника
    void CreateCenter(float Width, float Height, float Radius, const sf::Color& Color);

    // Создание углов
    void CreateCorners(float Width, float Height, float Radius, const sf::Color& Color);

    // Создание сторон
    void CreateSides(float Width, float Height, float Radius, const sf::Color& Color);

public:
    void Draw(sf::RenderWindow& Window) const;

private:
       
    // Центральный прямоугольник
    sf::RectangleShape Center;

    // Углы (четыре окружности)
    sf::CircleShape Corners[4];
    sf::RectangleShape Top, Bottom, Left, Right;
};
