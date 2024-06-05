#pragma once
#include "../Constants.h"

class AFpsManager
{
public:
    AFpsManager() = default;
    ~AFpsManager() = default;
    
    void InitFpsText();
    void UpdateFpsText(float DeltaTime);
    void SetPositionFpsText(const sf::Vector2f& NewPosition);
    void DrawFpsText(sf::RenderWindow& Window) const;

private:
    // Делаем отображение количества кадров в секунду(FPS)
    sf::Text FPSText;
    sf::Font FPSFont;
};
