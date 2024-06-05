﻿#include "GameState.h"


AGameState::AGameState()
    : ScreenRect{{0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}}
      , EnemyPtr(new AEnemy(sf::Vector2f(450.f, 470.f)))
      , PlayerPtr(new APlayer)
      , GameMapPtr(new AGameMap(*PlayerPtr))
      , SpriteManagerPtr(new ASpriteManager)
      , CollisionManagerPtr(new ACollisionManager(*EnemyPtr, *PlayerPtr, *GameMapPtr))
      , FpsManagerPtr(new AFpsManager)
{
}

AGameState::~AGameState()
{
    delete EnemyPtr;
    delete PlayerPtr;
    delete SpriteManagerPtr;
    delete CollisionManagerPtr;
    delete FpsManagerPtr;

    for (const ABullet* Bullet : BulletsVectorPtr)
    {
        delete Bullet;
    }

    BulletsVectorPtr.clear();
}

void AGameState::InitGame()
{
    constexpr int CapacityVector = 200;

    GameMapPtr->InitGameMap();
    EnemyPtr->InitEnemy(*SpriteManagerPtr, 300.f);
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
    FpsManagerPtr->InitFpsText();

    BulletsVectorPtr.reserve(CapacityVector);
}

void AGameState::UpdateGameplay(float DeltaTime)
{
    // Обновлять состояние передвижения персонажа
    PlayerPtr->UpdatePlayerMove(DeltaTime, *CollisionManagerPtr);

    // Обновлять состояние передвижения пуль
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        Bullet->UpdateBulletPosition(DeltaTime);
    }

    // Проверяем с чем сталкиваются пули
    CollisionManagerPtr->CheckAllBulletCollisions(BulletsVectorPtr);

    FpsManagerPtr->UpdateFpsText(DeltaTime);
}

void AGameState::UpdateInput(float DeltaTime)
{
    EnemyPtr->UpdateEnemyMove(DeltaTime, *PlayerPtr);
    
    // Проверяем нажатые клавиши и обновляем состояние персонажа
    PlayerPtr->HandlePlayerMove(DeltaTime);

    // Сделать зарежку между каждый выстрелом
    float ElapsedSeconds = DelayShotTimerHandle.getElapsedTime().asSeconds();
    if (ElapsedSeconds >= 0.05f)
    {
        // Делаем выстрел, если нажали левую кнопку мыши
        PlayerPtr->HandlePlayerShoots(BulletsVectorPtr, *SpriteManagerPtr);
        DelayShotTimerHandle.restart();
    }
}

void AGameState::UpdateCamera(sf::RenderWindow& Window)
{
    // Фокусируем игровую камеру на главном персонаже
    ViewPlayer = Window.getView();
    ViewPlayer.setSize(Window.getDefaultView().getSize() * ZOOM_FACTOR);
    ViewPlayer.setCenter(PlayerPtr->GetPlayerPossition());

    // Устанавливаем положение ФПС в новую позицию, слева ссверху от персонажа
    FpsManagerPtr->SetPositionFpsText(sf::Vector2f(ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2) + 10.f,
                                                   ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2) + 10.f));
    
    Window.setView(ViewPlayer);
}

void AGameState::DrawGame(sf::RenderWindow& Window) const
{
    // Рисовать всю карту
    GameMapPtr->DrawGameMap(Window, 1.f);

    // Рисовать остальные объекты
    EnemyPtr->DrawEnemy(Window);
    PlayerPtr->DrawPlayer(Window);

    // Рисовать пули
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        Bullet->DrawBullet(Window);
    }

    // Показывать количество ФПС на экране
    FpsManagerPtr->DrawFpsText(Window);
}
