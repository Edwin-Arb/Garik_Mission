#include <iostream>
#include "GameMain/GameState.h"
#include "GameMap/GameMap.h"


int main()
{
    sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Garik Mission");

    sf::Clock GameClock;

    AGameState* Game = new AGameState;
    Game->InitGame();

    while (Window.isOpen())
    {
        // Делаем задержку между кадрами, чтобы игра работала на всех компьютерах одинаково
        sleep(sf::milliseconds(1));
        float DeltaTime = GameClock.getElapsedTime().asSeconds();

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
        Game->UpdateInput(DeltaTime);

        // Обновлять игровое состояние
        Game->UpdateGameplay(DeltaTime);

        // Обновлять положение камеры, слежение за играком
        Game->UpdateCamera(Window);

        // Очищать экран
        Window.clear();
        
        // Отрисовываем игру
        Game->DrawGame(Window);

        Window.display();
    }
    return 0;
}
