#include "GameState.h"
#include "GameEngine.h"

AGameState::AGameState()
    : ScreenRect{{0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}},
      Enemy(new AEnemy),
      Player(new APlayer)
{}

AGameState::~AGameState()
{
    delete Enemy;
    delete Player;
    
    for (const ABullet *bullet : BulletsVectorPtr)
    {
        delete bullet;
    }
    
    BulletsVectorPtr.clear();
}

void AGameState::InitGame() const
{
    Enemy->InitEnemy();
    Player->InitPlayer();
}

// void AGameState::HandleUserInput(const sf::Event &event)
// {
//     
//         if(event.key.code == (sf::Keyboard::W))
//         {
//             Player.SetPlayerDirection(EPawnDirection::EPD_Up);
//         }
//         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
//         {
//             Player.SetPlayerDirection(EPawnDirection::EPD_Left);
//         }
//         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
//         {
//             Player.SetPlayerDirection(EPawnDirection::EPD_Down);
//         }
//         else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
//         {
//             Player.SetPlayerDirection(EPawnDirection::EPD_Right);
//         }
//     if (event.type == sf::Event::KeyPressed)
//     {
//         if (event.key.code == sf::Keyboard::Space)
//         {
//             Player.SetPlayerDirection(EPawnDirection::EPD_Jump);
//         }
//     }
//
// }

void AGameState::UpdateGameplay(float DeltaTime)
{
    // Обновлять состояние передвижения персонажа
    Player->UpdatePlayerMove(DeltaTime);

    // Сделать зарежку между каждый выстрелом
    float elapsedSeconds = DelayShotTimerHandle.getElapsedTime().asSeconds();
    if (elapsedSeconds >= 0.05f)
    {
        // Делаем выстрел, если нажали левую кнопку мыши
        AGameEngine::PlayerShoot(BulletsVectorPtr, Player->GetPlayerRect());
        DelayShotTimerHandle.restart();
    }

    // Проверяем с чем сталкиваются пули
    AGameEngine::CheckBulletCollision(BulletsVectorPtr, *Enemy, DeltaTime);
}

void AGameState::DrawGame(sf::RenderWindow& Window) const
{
    // Draw everything
    Enemy->DrawEnemy(Window);
    Player->DrawPlayer(Window);
    
    // Рисовать пули
    for (ABullet* bullet : BulletsVectorPtr)
    {
        bullet->DrawBullet(Window);
    }
}
