#include "GameState.h"

#include <iostream>


AGameState::AGameState()
    : ScreenRect{{0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}}
      , PlayerPtr(new APlayer)
      , GameMapPtr(new AGameMap(*PlayerPtr))
      , SpriteManagerPtr(new ASpriteManager)
      , CollisionManagerPtr(new ACollisionManager(*PlayerPtr, *GameMapPtr))
      , FpsManagerPtr(new AFpsManager)
{
}

AGameState::~AGameState()
{
    delete PlayerPtr;
    delete SpriteManagerPtr;
    delete CollisionManagerPtr;
    delete FpsManagerPtr;

    // Очищаем вектор врагов
    for (const AEnemy* Enemy : EnemysVectorPtr)
    {
        delete Enemy;
    }
    EnemysVectorPtr.clear();

    // Очишаем вектор пуль, если какие-то пули остались в памяти
    for (const ABullet* Bullet : BulletsVectorPtr)
    {
        delete Bullet;
    }
    BulletsVectorPtr.clear();
}

void AGameState::InitGame()
{
    // TODO Изменить значение, когда игра будет законченна, на количество врагов
    constexpr int CapacityVectorEnemy = 1;
    constexpr int CapacityVectorBullet = 50;

    GameMapPtr->InitGameMap();
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
    FpsManagerPtr->InitFpsText();

    // Инициализация врагов
    EnemysVectorPtr.reserve(CapacityVectorEnemy);
    for (int i = 0; i < CapacityVectorEnemy; ++i)
    {
        AEnemy* Enemy = new AEnemy(400, {450.f, 470.f});
        Enemy->InitEnemy(*SpriteManagerPtr);
        EnemysVectorPtr.emplace_back(Enemy);
    }

    BulletsVectorPtr.reserve(CapacityVectorBullet);
}

void AGameState::UpdateGameplay(float DeltaTime)
{
    // Обновлять состояние передвижения персонажа
    PlayerPtr->UpdatePlayerMove(DeltaTime, *CollisionManagerPtr);

    // Обновление состояния врагов
    for (AEnemy* Enemy : EnemysVectorPtr)
    {
        Enemy->UpdateEnemyMove(DeltaTime, *PlayerPtr, *GameMapPtr);
    }

    // Обновлять состояние передвижения пуль
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        Bullet->UpdateBulletPosition(DeltaTime);
    }

    // Проверяем с чем сталкиваются пули
    CollisionManagerPtr->CheckAllBulletCollisions(BulletsVectorPtr, EnemysVectorPtr);

    // Обновлять ФПС игры
    FpsManagerPtr->UpdateFpsText(DeltaTime);
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

        // Делаем выстрел, если враг обнаружил персонажа 
        for (const AEnemy* Enemy : EnemysVectorPtr)
        {
            Enemy->EnemyShoot(BulletsVectorPtr, *SpriteManagerPtr);
        }

        // Обновляем время, чтобы корректно работал интервал времени между выстрелами 
        DelayShotTimerHandle.restart();
    }
}

void AGameState::UpdateCamera(sf::RenderWindow& Window)
{
    // Фокусируем игровую камеру на главном персонаже
    ViewPlayer = Window.getView();
    ViewPlayer.setSize(Window.getDefaultView().getSize() * ZOOM_FACTOR);
    ViewPlayer.setCenter(PlayerPtr->GetPlayerPossition());

    // TODO тесты с камерой, надо будет сделать, чтобы левый и правый край не заходил дальше карты 
    // if(ViewPlayer.getCenter().x < 0.f)
    // {
    //     ViewPlayer.setCenter(0.f, 0.f);
    // }

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
    PlayerPtr->DrawPlayer(Window);

    // Рисовать врагов
    for (AEnemy* Enemy : EnemysVectorPtr)
    {
        Enemy->DrawEnemy(Window);
    }

    // Рисовать пули
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        Bullet->DrawBullet(Window);
    }

    // Показывать количество ФПС на экране
    FpsManagerPtr->DrawFpsText(Window);
}
