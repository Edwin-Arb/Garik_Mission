#include "GameState.h"
#include <iostream>

#include "../Enemy/BaseEnemy.h"
#include "../Enemy/BossEnemy.h"


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
    for (auto Enemy : EnemyVectorPtr)
    {
        delete Enemy;
    }
    EnemyVectorPtr.clear();

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
    constexpr int CapacityVectorEnemy = 30; // Вместимость вектора врагов
    constexpr int CapacityVectorBullet = 50; // Вместимость вектора пуль

    GameMapPtr->InitGameMap();
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
    FpsManagerPtr->InitFpsText();

    // Инициализация врагов
    EnemyVectorPtr.reserve(CapacityVectorEnemy);

    for (const sf::Vector2f& EnemyPosition : GameMapPtr->SpawnBaseEnemyPosition)
    {
        ABaseEnemy* BaseEnemy = new ABaseEnemy(80.f, EnemyPosition); // Указать начальную позицию врага
        BaseEnemy->InitEnemy(*SpriteManagerPtr);
        EnemyVectorPtr.push_back(BaseEnemy);
    }

    for (const sf::Vector2f& BossPosition : GameMapPtr->SpawnBossEnemyPosition)
    {
        ABossEnemy* Boss = new ABossEnemy(80.f, BossPosition); // Указать начальную позицию врага
        Boss->InitEnemy(*SpriteManagerPtr);
        EnemyVectorPtr.push_back(Boss);
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
    if (ElapsedSeconds >= 0.3f)
    {
        // Выстрел персонажа при нажатии левой кнопки мыши
        PlayerPtr->HandlePlayerShoots(BulletsVectorPtr, *SpriteManagerPtr);

        // Выстрел врага при обнаружении персонажа
        for (auto Enemy : EnemyVectorPtr)
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
    PlayerPtr->UpdatePlayerMove(DeltaTime);

    // Проверка столкновения персонажа с объектами, коллизиями карты
    CollisionManagerPtr->HandlePawnCollisionWithGameMap(PlayerPtr->GetActorCollisionRect(),
                                                        PlayerPtr->GetActorVelocity(),
                                                        PlayerPtr->GetPawnCanJump(), PlayerPtr->GetIsOnLadder());

    // Обновление движения врагов
    for (auto Enemy : EnemyVectorPtr)
    {
        Enemy->UpdateEnemyMove(DeltaTime, *PlayerPtr, *GameMapPtr, *CollisionManagerPtr);
    }

    // Обновление движения пуль
    for (auto Bullet : BulletsVectorPtr)
    {
        Bullet->UpdateBulletPosition(DeltaTime);
    }

    // Проверка столкновений пуль
    CollisionManagerPtr->CheckAllBulletCollisions(BulletsVectorPtr, EnemyVectorPtr, *PlayerPtr);

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
    GameMapPtr->DrawGameMap(Window);

    // Отрисовка персонажа
    PlayerPtr->DrawActor(Window);

    // Отрисовка врагов
    for (auto Enemy : EnemyVectorPtr)
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
