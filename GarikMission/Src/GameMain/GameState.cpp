#include "GameState.h"
#include <iostream>


/**
 * @brief Конструктор класса AGameState.
 * 
 * Инициализирует игровые объекты и менеджеров.
 */
AGameState::AGameState()
    : ScreenRect({0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}), // Инициализация экранной области игры
      PlayerPtr(new APlayer),
      GameMapPtr(new AGameMap(*PlayerPtr)),
      SpriteManagerPtr(new ASpriteManager),
      CollisionManagerPtr(new ACollisionManager(*PlayerPtr, *GameMapPtr)),
      FpsManagerPtr(new AFpsManager)
{
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
}

/**
 * @brief Деструктор класса AGameState.
 * 
 * Освобождает память, удаляя игровые объекты и очищая вектора.
 */
AGameState::~AGameState()
{
    // Освобождение памяти указателей на игровые объекты
    delete PlayerPtr;
    delete GameMapPtr;
    delete SpriteManagerPtr;
    delete CollisionManagerPtr;
    delete FpsManagerPtr;

    // Очищаем вектор врагов
    for (auto Enemy : EnemysVectorPtr)
    {
        delete Enemy;
    }
    EnemysVectorPtr.clear();

    // Очищаем вектор пуль
    for (auto Bullet : BulletsVectorPtr)
    {
        delete Bullet;
    }
    BulletsVectorPtr.clear();
}

/**
 * @brief Инициализация игры.
 * 
 * Загружает игровую карту, персонажа, врагов и устанавливает FPS.
 */
void AGameState::InitGame()
{
    // TODO: Изменить значение, когда игра будет завершена, на количество врагов
    constexpr int CapacityVectorEnemy = 1; // Вместимость вектора врагов
    constexpr int CapacityVectorBullet = 50; // Вместимость вектора пуль

    GameMapPtr->InitGameMap();
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
    FpsManagerPtr->InitFpsText();

    // Инициализация врагов
    EnemysVectorPtr.reserve(CapacityVectorEnemy);
    for (int i = 0; i < CapacityVectorEnemy; ++i)
    {
        AEnemy* Enemy = new AEnemy(400, {450.f, 470.f}); // Указать начальную позицию врага
        Enemy->InitEnemy(*SpriteManagerPtr);
        EnemysVectorPtr.push_back(Enemy);
    }

    BulletsVectorPtr.reserve(CapacityVectorBullet); // Резервирование места для пуль
}

/**
 * @brief Обновление пользовательского ввода.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 */
void AGameState::UpdateInput(float DeltaTime)
{
    // Проверяем нажатые клавиши и обновляем состояние персонажа
    PlayerPtr->HandlePlayerMove(DeltaTime);

    // Задержка между выстрелами
    float ElapsedSeconds = DelayShotTimerHandle.getElapsedTime().asSeconds();
    if (ElapsedSeconds >= 0.1f)
    {
        // Выстрел персонажа при нажатии левой кнопки мыши
        PlayerPtr->HandlePlayerShoots(BulletsVectorPtr, *SpriteManagerPtr);

        // Выстрел врага при обнаружении персонажа
        for (auto Enemy : EnemysVectorPtr)
        {
            Enemy->EnemyShoot(BulletsVectorPtr, *SpriteManagerPtr);
        }

        // Сброс таймера задержки между выстрелами
        DelayShotTimerHandle.restart();
    }
}

/**
 * @brief Обновление игрового процесса.
 * 
 * @param DeltaTime Время, прошедшее с последнего обновления.
 */
void AGameState::UpdateGameplay(float DeltaTime)
{
    // Обновление движения персонажа
    PlayerPtr->UpdatePlayerMove(DeltaTime, *CollisionManagerPtr);

    // Обновление движения врагов
    for (auto Enemy : EnemysVectorPtr)
    {
        Enemy->UpdateEnemyMove(DeltaTime, *PlayerPtr, *GameMapPtr);
    }

    // Обновление движения пуль
    for (auto Bullet : BulletsVectorPtr)
    {
        Bullet->UpdateBulletPosition(DeltaTime);
    }

    // Проверка столкновений пуль
    CollisionManagerPtr->CheckAllBulletCollisions(BulletsVectorPtr, EnemysVectorPtr, *PlayerPtr);

    // Обновление отображения FPS
    FpsManagerPtr->UpdateFpsText(DeltaTime);
}

/**
 * @brief Обновление камеры игры.
 * 
 * @param Window Окно игры для установки камеры.
 */
void AGameState::UpdateCamera(sf::RenderWindow& Window)
{
    // Фокусировка камеры на игроке
    ViewPlayer = Window.getView();
    ViewPlayer.setSize(Window.getDefaultView().getSize() * ZOOM_FACTOR);
    ViewPlayer.setCenter(PlayerPtr->GetActorPosition());

    // Установка позиции FPS текста относительно персонажа
    FpsManagerPtr->SetPositionFpsText(sf::Vector2f(ViewPlayer.getCenter().x - (ViewPlayer.getSize().x / 2) + 10.f,
                                                   ViewPlayer.getCenter().y - (ViewPlayer.getSize().y / 2) + 10.f));

    Window.setView(ViewPlayer);
}

/**
 * @brief Отрисовка игровых объектов.
 * 
 * @param Window Окно игры для отрисовки объектов.
 */
void AGameState::DrawGame(sf::RenderWindow& Window) const
{
    // Отрисовка карты
    GameMapPtr->DrawGameMap(Window, ViewPlayer);

    // Отрисовка персонажа
    PlayerPtr->DrawActor(Window);

    // Отрисовка врагов
    for (auto Enemy : EnemysVectorPtr)
    {
        Enemy->DrawActor(Window);
    }

    // Отрисовка пуль
    for (auto Bullet : BulletsVectorPtr)
    {
        Bullet->DrawActor(Window);
    }

    // Отрисовка FPS
    FpsManagerPtr->DrawFpsText(Window);
}
