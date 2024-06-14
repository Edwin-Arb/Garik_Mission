#include "FpsManager.h"
#include "assert.h"

void AFpsManager::InitFpsText()
{
    // Убеждаемся, что шрифт успешно загружен
    assert(FPSFont.loadFromFile(ASSETS_PATH + "Fonts/Roboto-Bold.ttf"));

    // Устанавливаем шрифт для текста FPS
    FPSText.setFont(FPSFont);

    // Устанавливаем размер символов текста FPS
    FPSText.setCharacterSize(20);

    // Устанавливаем начальную позицию текста FPS
    FPSText.setPosition(10.f, 10.f);

    // Устанавливаем цвет текста FPS
    FPSText.setFillColor(sf::Color::Yellow);
}

void AFpsManager::UpdateFpsText(float DeltaTime)
{
    // Обновляем текст, отображающий количество FPS
    FPSText.setString("FPS: " + std::to_string(static_cast<int>(1 / DeltaTime)));
}

void AFpsManager::SetPositionFpsText(const sf::Vector2f& NewPosition)
{
    // Устанавливаем новую позицию текста FPS
    FPSText.setPosition(NewPosition);
}

void AFpsManager::DrawFpsText(sf::RenderWindow& Window) const
{
    // Отрисовываем текст FPS на указанном окне
    Window.draw(FPSText);
}
