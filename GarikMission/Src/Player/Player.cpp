#include "Player.h"
#include "../Manager/CollisionManager.h"


/**
 * @brief Конструктор по умолчанию для класса APlayer.
 * Инициализирует начальные значения членов класса.
 */
APlayer::APlayer()
    : bCanJump(true)                             // Персонаж может прыгать при инициализации
      , bIsMoveRight(true)                       // Персонаж смотрит вправо при инициализации
      , bIsOnLadder(false)                       // Персонаж не на лестнице при инициализации
      , PlayerHealth(400.f)
      , PlayerSpeed(PAWN_SPEED)
      , PlayerJumpSpeed(1500.f)
      , PlayerVelocity({0.f, 0.f})
      , PlayerRect({
          100.f, 400.f,
          PLAYER_SIZE.x * DRAW_SCALE.x,
          PLAYER_SIZE.y * DRAW_SCALE.y
      })                                        // Прямоугольник персонажа с учетом масштабирования
      , PlayerTexturePtr(new sf::Texture)
      , HealthBarTexturePtr(new sf::Texture)
      , HealthBarPtr(new AHealthBar)
      , PlayerRectCollision(sf::Vector2f(PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y))
{
}

/**
 * @brief Деструктор класса APlayer.
 * Освобождает выделенные ресурсы.
 */
APlayer::~APlayer()
{
    delete PlayerTexturePtr;
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
    // Загрузка текстур персонажа и шкалы здоровья
    assert(PlayerTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/Player.png"));
    assert(HealthBarTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/HealthBarPlayer.png"));

    // Инициализация шкалы здоровья
    HealthBarPtr->InitHealthBar(165, 25, sf::Color::Red, sf::Color::Yellow, SpriteManager);
    HealthBarSprite.setTexture(*HealthBarTexturePtr);
    HealthBarSprite.setScale(0.2f, 0.2f);

    // Создание спрайта персонажа
    PlayerSprite.setTexture(*PlayerTexturePtr);
    PlayerSprite.setTextureRect(sf::IntRect(19, 0,
                                            static_cast<int>(PLAYER_SIZE.x),
                                            static_cast<int>(PLAYER_SIZE.y)));

    // Установка масштаба спрайта персонажа
    SpriteManager.SetSpriteSize(PlayerSprite, PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y);

    // Установка центра спрайта персонажа
    SpriteManager.SetSpriteRelativeOrigin(PlayerSprite, 0.5f, 0.5f);

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
                                                  sf::Vector2f(PlayerRect.left + SpawnBulletOffsetX,
                                                               PlayerRect.top + SpawnBulletOffsetY),
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
    PlayerVelocity.x = 0.f;

    // Движение влево
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        bIsMoveRight = false;
        PlayerVelocity.x = -PlayerSpeed * DeltaTime;
        PlayerSprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
    }
    // Движение вправо
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        bIsMoveRight = true;
        PlayerVelocity.x = PlayerSpeed * DeltaTime;
        PlayerSprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
    }
    // Движение вверх (подъем по лестнице)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (bIsOnLadder)
        {
            PlayerVelocity.y = PlayerSpeed * DeltaTime;
        }
    }
    // Движение вниз (спуск по лестнице)
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (bIsOnLadder)
        {
            PlayerVelocity.y = -PlayerSpeed * DeltaTime;
        }
    }

    // Прыжок
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (bCanJump)
        {
            bCanJump = false;
            PlayerVelocity.y = PlayerJumpSpeed * DeltaTime;
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
    PlayerVelocity.y += GRAVITY * DeltaTime;

    // Обновление позиции персонажа на основе скорости
    PlayerRect.left += PlayerVelocity.x;
    PlayerRect.top -= PlayerVelocity.y;

    // Обработка столкновений персонажа с объектами карты
    CollisionManager.HandlePlayerCollisionWithGameMap(PlayerRect, PlayerVelocity, bCanJump, bIsOnLadder);

    // Обновление позиции спрайта персонажа и его коллизии
    PlayerDrawPosition = {
        PlayerRect.left + (PLAYER_SIZE.x * DRAW_SCALE.x) / 2.f,
        PlayerRect.top + (PLAYER_SIZE.y * DRAW_SCALE.y) / 2.f
    };

    // Установка позиции спрайта персонажа
    PlayerSprite.setPosition(PlayerDrawPosition);

    // Установка позиции прямоугольника коллизии персонажа
    PlayerRectCollision.setPosition(PlayerDrawPosition);
}

/**
 * @brief Обновление состояния шкалы здоровья персонажа.
 * Рассчитывает текущее состояние шкалы и её положение на экране.
 */
void APlayer::UpdatePlayerHealthBar()
{
    // Обновление значений шкалы здоровья
    HealthBarPtr->Update(PlayerHealth, 400);
    HealthBarPtr->SetPosition({PlayerDrawPosition.x - 635.f, PlayerDrawPosition.y - 360.f});
    HealthBarSprite.setPosition(PlayerDrawPosition.x - 760.f, PlayerDrawPosition.y - 380.f);

    // TODO: Временное восстановление здоровья для тестирования
    if (PlayerHealth <= DEATH)
    {
        PlayerHealth = 400;
    }
}

/**
 * @brief Установка новой скорости персонажа.
 * 
 * @param NewVelocity Новая скорость персонажа.
 */
void APlayer::SetPlayerVelocity(const sf::Vector2f& NewVelocity)
{
    PlayerVelocity = NewVelocity;
}

/**
 * @brief Установка нового прямоугольника коллизии для персонажа.
 * 
 * @param NewRect Новый прямоугольник коллизии.
 */
void APlayer::SetPlayerRect(const sf::FloatRect& NewRect)
{
    PlayerRect = NewRect;
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
 * @brief Получение текущей позиции персонажа.
 * 
 * @return Текущая позиция персонажа.
 */
sf::Vector2f APlayer::GetPlayerPosition() const
{
    return PlayerDrawPosition;
}

/**
 * @brief Получение текущего прямоугольника коллизии персонажа.
 * 
 * @return Текущий прямоугольник коллизии персонажа.
 */
sf::FloatRect APlayer::GetPlayerRect() const
{
    return PlayerRect;
}

/**
 * @brief Отрисовка персонажа на экране.
 * Отображает спрайт персонажа, его коллизию и шкалу здоровья.
 * 
 * @param Window Окно, на котором происходит отрисовка.
 */
void APlayer::DrawPlayer(sf::RenderWindow& Window)
{
    // Отрисовка спрайта и коллизии персонажа
    Window.draw(PlayerRectCollision);
    Window.draw(PlayerSprite);

    // Отрисовка шкалы здоровья
    UpdatePlayerHealthBar();
    Window.draw(HealthBarSprite);
    HealthBarPtr->Draw(Window);
}