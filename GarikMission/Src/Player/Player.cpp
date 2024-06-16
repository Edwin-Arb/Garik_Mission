#include "Player.h"
#include "../Manager/CollisionManager.h"


/**
 * @brief Конструктор по умолчанию для класса APlayer.
 * Инициализирует начальные значения членов класса.
 */
APlayer::APlayer()
    : bCanJump(true) // Персонаж может прыгать при инициализации
      , bIsMoveRight(true) // Персонаж смотрит вправо при инициализации
      , bIsOnLadder(false) // Персонаж не на лестнице при инициализации
      , PlayerHealth(400.f)
      , PlayerSpeed(PAWN_SPEED)
      , PlayerJumpSpeed(1500.f)

      //, PlayerTexturePtr(new sf::Texture)
      , HealthBarPtr(new AHealthBar)
      , HealthBarTexturePtr(new sf::Texture)
      , PlayerRectCollision(sf::Vector2f(PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y))
{
    ActorVelocity = {0.f, 0.f};

    // Прямоугольник персонажа с учетом масштабирования
    ActorCollisionRect = {
        100.f, 400.f,
        PLAYER_SIZE.x * DRAW_SCALE.x,
        PLAYER_SIZE.y * DRAW_SCALE.y
    };
}

/**
 * @brief Деструктор класса APlayer.
 * Освобождает выделенные ресурсы.
 */
APlayer::~APlayer()
{
    delete HealthBarPtr;
    delete HealthBarTexturePtr;
}

/**
 * @brief Инициализация персонажа.
 * Загружает текстуры и настраивает спрайты и шкалу здоровья.
 * 
 * @param SpriteManager Менеджер спрайтов для управления отображением.
 */
void APlayer::InitPlayer(ASpriteManager& SpriteManager)
{
    // Инициализировать текстуру для персонажа и создать спрайт для него
    InitActorTexture(ASSETS_PATH + "MainTiles/Player.png",
                     sf::IntRect(19, 0, static_cast<int>(PLAYER_SIZE.x), static_cast<int>(PLAYER_SIZE.y)),
                     {PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y},
                     {0.5f, 0.5f},
                     SpriteManager);

    // Загрузка текстур шкалы здоровья
    assert(HealthBarTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/HealthBarPlayer.png"));

    // Инициализация шкалы здоровья
    HealthBarPtr->InitHealthBar(165, 25, sf::Color::Red, sf::Color::Yellow, SpriteManager);
    HealthBarSprite.setTexture(*HealthBarTexturePtr);
    HealthBarSprite.setScale(0.2f, 0.2f);

    // Установка центра прямоугольника коллизии персонажа
    SpriteManager.SetShapeRelativeOrigin(PlayerRectCollision, 0.5f, 0.5f);
}

/**
 * @brief Обработка выстрелов персонажа.
 * Создает новый снаряд, если нажата левая кнопка мыши.
 * 
 * @param BulletsVectorPtr Указатель на вектор снарядов для добавления нового снаряда.
 * @param SpriteManager Менеджер спрайтов для управления отображением.
 */
void APlayer::HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const
{
    // Стрельба при нажатии левой кнопки мыши
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? 90.f : 0.f;
        constexpr float SpawnBulletOffsetY = 85.f;
        
        // Создание нового снаряда и добавление его в вектор снарядов
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, EBulletType::EBT_ShootAtEnemy,
                                      sf::Vector2f(ActorCollisionRect.left + SpawnBulletOffsetX,
                                                   ActorCollisionRect.top + SpawnBulletOffsetY),
                                      SpriteManager));
    }
}

/**
 * @brief Обработка движения персонажа.
 * Устанавливает скорость персонажа в зависимости от нажатых клавиш.
 * 
 * @param DeltaTime Время, прошедшее с предыдущего кадра.
 */
void APlayer::HandlePlayerMove(float DeltaTime)
{
    // Сброс скорости по оси X
    ActorVelocity.x = 0.f;

    // Движение влево
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        bIsMoveRight = false;
        ActorVelocity.x = -PlayerSpeed * DeltaTime;
        ActorSprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
    }
    // Движение вправо
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        bIsMoveRight = true;
        ActorVelocity.x = PlayerSpeed * DeltaTime;
        ActorSprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
    }
    // Движение вверх (подъем по лестнице)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (bIsOnLadder)
        {
            ActorVelocity.y = PlayerSpeed;
        }
    }

    // TODO: Возможно придёться убрать, так как есть гравитация, и спуск по лестнице бесмысленный
    // Движение вниз (спуск по лестнице)
    // else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    // {
    //     if (bIsOnLadder)
    //     {
    //         PlayerVelocity.y = -PlayerSpeed * DeltaTime;
    //     }
    // }

    // Прыжок
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (bCanJump)
        {
            bCanJump = false;
            ActorVelocity.y = PlayerJumpSpeed * DeltaTime;
        }
    }
}

/**
 * @brief Обновление позиции и состояния персонажа.
 * Включает в себя обработку гравитации и проверку столкновений с объектами.
 * 
 * @param DeltaTime Время, прошедшее с предыдущего кадра.
 * @param CollisionManager Менеджер коллизий для обработки столкновений персонажа с объектами карты.
 */
void APlayer::UpdatePlayerMove(float DeltaTime, const ACollisionManager& CollisionManager)
{
    // Применение гравитации для падения персонажа
    ActorVelocity.y += GRAVITY * DeltaTime;

    // Обновление позиции персонажа на основе скорости
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;

    // Обработка столкновений персонажа с объектами карты
    CollisionManager.HandlePlayerCollisionWithGameMap(ActorCollisionRect, ActorVelocity, bCanJump, bIsOnLadder);

    // Обновление позиции спрайта персонажа и его коллизии
    ActorDrawPosition = {
        ActorCollisionRect.left + (PLAYER_SIZE.x * DRAW_SCALE.x) / 2.f,
        ActorCollisionRect.top + (PLAYER_SIZE.y * DRAW_SCALE.y) / 2.f
    };

    // Установка позиции спрайта персонажа
    ActorSprite.setPosition(ActorDrawPosition);

    // Установка позиции прямоугольника коллизии персонажа
    PlayerRectCollision.setPosition(ActorDrawPosition);
}

/**
 * @brief Обновление состояния шкалы здоровья персонажа.
 * Рассчитывает текущее состояние шкалы и её положение на экране.
 */
void APlayer::UpdatePlayerHealthBar()
{
    // Обновление значений шкалы здоровья
    HealthBarPtr->Update(PlayerHealth, 400);
    HealthBarPtr->SetPosition({ActorDrawPosition.x - 635.f, ActorDrawPosition.y - 360.f});
    HealthBarSprite.setPosition(ActorDrawPosition.x - 760.f, ActorDrawPosition.y - 380.f);

    // TODO: Временное восстановление здоровья для тестирования
    if (PlayerHealth <= DEATH)
    {
        PlayerHealth = 400;
    }
}

/**
 * @brief Установка нового максимального значения здоровья персонажа.
 * 
 * @param NewPlayerHealth Новое максимальное значение здоровья.
 */
void APlayer::SetPlayerMaxHealth(int NewPlayerHealth)
{
    PlayerHealth -= NewPlayerHealth;
}

/**
 * @brief Получение текущего значения здоровья персонажа.
 * 
 * @return Текущее значение здоровья персонажа.
 */
float APlayer::GetPlayerMaxHealth() const
{
    return PlayerHealth;
}

/**
 * @brief Отрисовка персонажа на экране.
 * Отображает спрайт персонажа, его коллизию и шкалу здоровья.
 * 
 * @param Window Окно, на котором происходит отрисовка.
 */
void APlayer::DrawActor(sf::RenderWindow& Window)
{
    // Отрисовка спрайта и коллизии персонажа
    Window.draw(PlayerRectCollision);
    Window.draw(ActorSprite);

    // Отрисовка шкалы здоровья
    UpdatePlayerHealthBar();
    Window.draw(HealthBarSprite);
    HealthBarPtr->Draw(Window);
}