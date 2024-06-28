#include <iostream>
#include "GameMain/GameState.h"


int main()
{
    // Создаем главное окно игры с заданными размерами
    sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Garik Mission");
    Window.setFramerateLimit(120);

    // Создаем таймер игры
    sf::Clock GameClock;

    // Инициализируем игровое состояние
    AGameState* Game = new AGameState;
    Game->InitGame();

    // Главный цикл игры
    while (Window.isOpen())
    {
        // Задержка между кадрами для обеспечения одинаковой скорости на всех устройствах
        //sleep(sf::milliseconds(0.00001));

        // Получаем время прошедшее между кадрами
        float DeltaTime = GameClock.getElapsedTime().asSeconds();

        // Сбрасываем таймер для следующего кадра
        GameClock.restart();

        // Обрабатываем события окна
        sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // Закрываем окно, если пользователь нажал крестик
                Window.close();
            }
        }

        // Обновляем ввод пользователя
        Game->UpdateInput(DeltaTime);

        // Обновляем игровое состояние
        Game->UpdateGameplay(DeltaTime);

        // Обновляем положение камеры, следя за игроком
        Game->UpdateCamera(Window);

        // Очищаем экран от предыдущего кадра
        Window.clear();

        // Отрисовываем игровые объекты
        Game->DrawGame(Window);

        // Показываем отрисованные объекты в окне
        Window.display();
    }

    // Освобождаем память, выделенную под игровое состояние
    delete Game;

    return 0;
}
