#include <iostream>
#include "GameMain/GameState.h"
#include "GameMap/GameMap.h"


int main()
{    
    sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Garik Mission");
    
    // Делаем отображение количества кадров в секунду(FPS)
    sf::Text FPSText;
    sf::Font FPSFont;
    assert(FPSFont.loadFromFile(ASSETS_PATH + "Fonts/Roboto-Bold.ttf"));
    FPSText.setFont(FPSFont);
    FPSText.setCharacterSize(15);
    FPSText.setPosition(10.f, 10.f);
    
    sf::Clock GameClock;
        
    AGameState Game;
    Game.InitGame();
    
    while (Window.isOpen())
    {
        // Делаем задержку между кадрами, чтобы игра работала на всех компьютерах одинаково
        sleep(sf::milliseconds(1));
        float DeltaTime = GameClock.getElapsedTime().asSeconds();
        
        // Текст, который показывает количества кадров в секунду(FPS)
        FPSText.setString("FPS: " + std::to_string(static_cast<int>(1/DeltaTime)));

        //std::cout << DeltaTime << "\n"; // TODO для тестов
        
        // Обновляем таймер между кадрами
        GameClock.restart();
        
        // Проверяем ивенты во время открытия окна
        sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Window.close();
            }
        }

        // Проверяем какие клавиши нажал Игрок
        Game.UpdateInput(DeltaTime);
       
        // Обновлять игровое состояние
        Game.UpdateGameplay(DeltaTime);

        // Очищать экран
        Window.clear();
        
        // Отрисовываем игру
        Game.DrawGame(Window);
        Window.draw(FPSText);
        
        Window.display();
    }
    return 0;
}