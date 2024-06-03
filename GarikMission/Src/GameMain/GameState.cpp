#include "GameState.h"


AGameState::AGameState()
    : ScreenRect{{0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}}
      , EnemyPtr(new AEnemy)
      , PlayerPtr(new APlayer)
      , GameMapPtr(new AGameMap(*PlayerPtr))
      , SpriteManagerPtr(new ASpriteManager)
      , CollisionManagerPtr(new ACollisionManager(*EnemyPtr, *PlayerPtr, *GameMapPtr))
{
}

AGameState::~AGameState()
{
    delete EnemyPtr;
    delete PlayerPtr;
    delete SpriteManagerPtr;

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
    EnemyPtr->InitEnemy(*SpriteManagerPtr);
    PlayerPtr->InitPlayer(*SpriteManagerPtr);

    BulletsVectorPtr.reserve(CapacityVector);
}

void AGameState::UpdateGameplay(float DeltaTime)
{
    // Обновлять состояние передвижения персонажа
    PlayerPtr->UpdatePlayerMove(DeltaTime, *GameMapPtr);

    // Обновлять состояние передвижения пуль
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        Bullet->UpdateBulletPosition(DeltaTime);
    }

    // Проверяем с чем сталкиваются пули
    CollisionManagerPtr->CheckBulletCollision(BulletsVectorPtr);
}

void AGameState::UpdateInput(float DeltaTime)
{
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

void AGameState::DrawGame(sf::RenderWindow& Window) const
{
    GameMapPtr->DrawGameMap(Window, 1.f);

    // Рисовать остальные объекты
    EnemyPtr->DrawEnemy(Window);
    PlayerPtr->DrawPlayer(Window);

    // Рисовать пули
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        Bullet->DrawBullet(Window);
    }
}
