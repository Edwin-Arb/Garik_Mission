#include "GameState.h"
#include "GameEngine.h"

AGameState::AGameState()
    : screenRect{{0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}}
{}

// AGameState::~AGameState()
// {
//     //delete player;
// }

void AGameState::InitGame()
{
    enemy.InitEnemy();
    player.InitPlayer();
}

// void AGameState::HandleUserInput(const sf::Event &event)
// {
//     
//         if(event.key.code == (sf::Keyboard::W))
//         {
//             player.SetPlayerDirection(EPawnDirection::EPD_Up);
//         }
//         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//         {
//             player.SetPlayerDirection(EPawnDirection::EPD_Left);
//         }
//         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//         {
//             player.SetPlayerDirection(EPawnDirection::EPD_Down);
//         }
//         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//         {
//             player.SetPlayerDirection(EPawnDirection::EPD_Right);
//         }
//     if (event.type == sf::Event::KeyPressed)
//     {
//         if (event.key.code == sf::Keyboard::Space)
//         {
//             player.SetPlayerDirection(EPawnDirection::EPD_Jump);
//         }
//     }
//
// }

void AGameState::UpdateGameplay(const sf::Event &event, float delta_time)
{
    // Обновлять состояние передвижения персонажа
    player.UpdatePlayerMove(event, delta_time);

    // Сделать зарежку между каждый выстрелом
    float elapsedSeconds = delayShootTime.getElapsedTime().asSeconds();
    if (elapsedSeconds >= 0.5f)
    {
        // Делаем выстрел, если нажали левую кнопку мыши
        AGameEngine::PlayerShoot(bulletsVector, player.GetPlayerRect());
        delayShootTime.restart();
    }

    // Проверяем с чем сталкиваются пули
    AGameEngine::CheckBulletCollision(bulletsVector, enemy, delta_time);
    
}

void AGameState::DrawGame(sf::RenderWindow& window)
{
    // Draw everything
    enemy.DrawEnemy(window);
    player.DrawPlayer(window);
    
    // Рисовать пули
    for (ABullet& bullet : bulletsVector)
    {
        bullet.DrawBullet(window);
    }
}
