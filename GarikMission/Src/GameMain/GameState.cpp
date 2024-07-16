#include "GameState.h"
#include <iostream>
#include <random>

/**
 * @brief Конструктор класса AGameState.
 * 
 * Инициализирует игровые объекты и менеджеры.
 */
AGameState::AGameState()
    : ScreenRect({0.f, 0.f}, {SCREEN_WIDTH, SCREEN_HEIGHT}), // Инициализация экранной области игры
      PlayerPtr(new APlayer),
      GameMapPtr(new AGameMap(*PlayerPtr)),
      SpriteManagerPtr(new ASpriteManager),
      CollisionManagerPtr(new ACollisionManager(*PlayerPtr, *GameMapPtr)),
      ParticleSystemPtr(new AParticleSystemManager),
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
    delete ParticleSystemPtr;
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
    constexpr int CapacityVectorEnemy = 30; // Вместимость вектора врагов
    constexpr int CapacityVectorBullet = 50; // Вместимость вектора пуль

    GameMapPtr->InitGameMap();
    PlayerPtr->InitPlayer(*SpriteManagerPtr);
    FpsManagerPtr->InitFpsText();

    // Резервирование места для врагов
    EnemyVectorPtr.reserve(CapacityVectorEnemy);

    // Для случайного появления врагов из 3 видов
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 2);

    for (const sf::Vector2f& EnemyPosition : GameMapPtr->SpawnBaseEnemyPosition)
    {
        int EnemyType = dist(gen);
        AEnemy* BaseEnemy = nullptr;

        switch (EnemyType)
        {
        case 0:
            // Зеленый враг
            BaseEnemy = new ABaseGreenEnemy(80.f, EnemyPosition);
            break;

        case 1:
            // Красный враг
            BaseEnemy = new ABaseRedEnemy(80.f, EnemyPosition);
            break;

        case 2:
            // Синий враг
            BaseEnemy = new ABaseBlueEnemy(80.f, EnemyPosition);
            break;

        default:
            BaseEnemy = nullptr;
            break;
        }

        if (BaseEnemy)
        {
            BaseEnemy->InitEnemy(*SpriteManagerPtr);
            EnemyVectorPtr.emplace_back(BaseEnemy);
        }
    }

    for (const sf::Vector2f& BossPosition : GameMapPtr->SpawnBossEnemyPosition)
    {
        AEnemy* Boss = new ABossEnemy(80.f, BossPosition); // Указать начальную позицию врага
        Boss->InitEnemy(*SpriteManagerPtr);
        EnemyVectorPtr.emplace_back(Boss);
    }

    // Резервирование места для пуль
    BulletsVectorPtr.reserve(CapacityVectorBullet);
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
            Enemy->EnemyShoot(9.f, BulletsVectorPtr, *SpriteManagerPtr);
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
    // Обновление анимаций тайлов на карте
    GameMapPtr->UpdateAnimatedTiles(DeltaTime);

    // Обновление движения персонажа
    PlayerPtr->UpdatePlayerMove(DeltaTime);

    if (!PlayerPtr->GetIsDeathPlayer())
    {
    // Проверка столкновения персонажа с объектами, коллизиями карты
    CollisionManagerPtr->HandlePawnCollisionWithGameMap(PlayerPtr->GetActorCollisionRect(),
                                                        PlayerPtr->GetActorVelocity(),
                                                        PlayerPtr->GetPawnCanJump(),
                                                        PlayerPtr->GetIsOnLadder());
        
    }

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

    // TODO: Тест системы частиц. К концу проекта доделать реализацию.
    //ParticleSystemPtr->InitParticleSystem();

    // Проверка столкновений пуль
    CollisionManagerPtr->CheckAllBulletCollisions(BulletsVectorPtr, EnemyVectorPtr, *PlayerPtr, *ParticleSystemPtr);

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
    if (!PlayerPtr->GetIsDeathPlayer())
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
