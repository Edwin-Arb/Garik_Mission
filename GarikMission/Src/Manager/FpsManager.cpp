#include "FpsManager.h"
#include "assert.h"

void AFpsManager::InitFpsText()
{
    assert(FPSFont.loadFromFile(ASSETS_PATH + "Fonts/Roboto-Bold.ttf"));
    FPSText.setFont(FPSFont);
    FPSText.setCharacterSize(20);
    FPSText.setPosition(10.f, 10.f);
    FPSText.setFillColor(sf::Color::Yellow);
}

void AFpsManager::UpdateFpsText(float DeltaTime)
{
    // Текст, который показывает количества кадров в секунду(FPS)
    FPSText.setString("FPS: " + std::to_string(static_cast<int>(1 / DeltaTime)));
}

void AFpsManager::SetPositionFpsText(const sf::Vector2f& NewPosition)
{
    FPSText.setPosition(NewPosition);
}

void AFpsManager::DrawFpsText(sf::RenderWindow& Window) const
{
    Window.draw(FPSText);
}
