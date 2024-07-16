#include "Player.h"
#include "../Manager/CollisionManager.h"


/**
 * @brief Конструктор по умолчанию для класса APlayer.
 * Инициализирует начальные значения членов класса.
 */
APlayer::APlayer()
    : bIsMoveRight(true) // Персонаж смотрит вправо при инициализации
      , bIsOnLadder(false) // Персонаж не на лестнице при инициализации
      , bIsDeath(false) // Персонаж живой при старте игры
      , PlayerJumpSpeed(200.f) // Установка скорости прыжка персонажа
      , CurrentAnimation(nullptr) // Инициализация указателя на текущую анимацию
{
    // Персонаж может прыгать при инициализации
    bIsPawnJump = true;

    // Максимальное и текущие здоровья персонажа,
    // чтобы использовать для отслеживания шкалы здоровья.
    PawnMaxHealth = 400.f;
    PawnCurrentHealth = PawnMaxHealth;

    // Скорость персонажа
    PawnSpeed = 300;

    // Скорость и направление персонажа
    ActorVelocity = {0.f, 0.f};

    // Прямоугольник персонажа с учетом масштабирования
    ActorCollisionRect = {
        100.f, 600.f,
        PLAYER_SIZE.x * DRAW_SCALE.x,
        PLAYER_SIZE.y * DRAW_SCALE.y
    };
}

/**
 * @brief Инициализация персонажа.
 * Загружает текстуры и настраивает спрайты и шкалу здоровья.
 * 
 * @param SpriteManager Менеджер спрайтов для управления отображением.
 */
void APlayer::InitPlayer(ASpriteManager& SpriteManager)
{
    // Инициализация переменных для текстуры персонажа
    const std::string PlayerTexturePath = ASSETS_PATH + "MainTiles/Player.png";
    const sf::IntRect PlayerRectTexture = sf::IntRect(19, 0, static_cast<int>(PLAYER_SIZE.x),
                                                      static_cast<int>(PLAYER_SIZE.y));
    const sf::Vector2f PlayerSize = {PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y};
    const sf::Vector2f PlayerOrigin = {0.5f, 0.5f};

    // Инициализировать текстуру для персонажа и создать спрайт для него
    InitActorTexture(PlayerTexturePath, PlayerRectTexture,
                     PlayerSize, PlayerOrigin,
                     SpriteManager);

    // Инициализация переменных текстуры шкалы здоровья для персонажа
    const std::string PlayerTextureHealthBarPath = ASSETS_PATH + "MainTiles/HealthBarPlayer.png";
    const sf::Vector2f HealthBarSize = sf::Vector2f(12.f, 2.f);
    const sf::Vector2f HealthBarScale = sf::Vector2f(0.2f, 0.2f);
    const sf::Color HealthBarFrontColor = sf::Color::Green;
    const sf::Color HealthBarBackgroundColor = sf::Color(202, 196, 176);

    // Инициализация шкалы здоровья
    InitPawnHealthBar(PlayerTextureHealthBarPath, HealthBarSize, HealthBarScale,
                      HealthBarFrontColor, HealthBarBackgroundColor,
                      SpriteManager);


    // TODO: Test Animation
    // Анимация ожидания(Idle)
    IdleAnimation.AnimTexture.loadFromFile(PlayerTexturePath);
    IdleAnimation.FrameSpeed = 3.f;

    IdleAnimation.FrameRect.emplace_back(sf::IntRect(3, 0, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));
    IdleAnimation.FrameRect.emplace_back(sf::IntRect(PlayerRectTexture));

    // Анимация бега(Walk)
    WalkAnimation.AnimTexture.loadFromFile(PlayerTexturePath);

    WalkAnimation.FrameSpeed = 10.f;

    WalkAnimation.FrameRect.emplace_back(sf::IntRect(3, 16, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));

    WalkAnimation.FrameRect.emplace_back(sf::IntRect(17, 16, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));

    WalkAnimation.FrameRect.emplace_back(sf::IntRect(35, 16, static_cast<int>(PLAYER_SIZE.x),
                                                     static_cast<int>(PLAYER_SIZE.y)));

    // Анимация прыжка(Jump)
    // Анимация прыжка, когда персонаж летит вверх
    JumpUpAnimation.AnimTexture.loadFromFile(PlayerTexturePath);

    JumpUpAnimation.FrameSpeed = 0.f;
    JumpUpAnimation.FrameRect.emplace_back(sf::IntRect(2, 32, static_cast<int>(PLAYER_SIZE.x),
                                                       static_cast<int>(PLAYER_SIZE.y)));

    // Анимация прыжка, когда персонаж уже прыгнул и летит вниз
    JumpDownAnimation.AnimTexture.loadFromFile(PlayerTexturePath);

    JumpDownAnimation.FrameSpeed = 2.f;

    JumpDownAnimation.FrameRect.emplace_back(sf::IntRect(18, 33, static_cast<int>(PLAYER_SIZE.x),
                                                         static_cast<int>(PLAYER_SIZE.y)));

    JumpDownAnimation.FrameRect.emplace_back(sf::IntRect(35, 33, static_cast<int>(PLAYER_SIZE.x),
                                                         static_cast<int>(PLAYER_SIZE.y)));

    JumpDownAnimation.FrameRect.emplace_back(sf::IntRect(51, 33, static_cast<int>(PLAYER_SIZE.x),
                                                         static_cast<int>(PLAYER_SIZE.y)));


    // Анимация смерти персонажа
    DeathAnimation.FrameRect.emplace_back(sf::IntRect(0, 50, 15, 60));
}

/**
* @brief Получить по ссылке данные, если персонаж находить рядом с лестницей и может карабкаться по ней.
* 
* @return Текущие состояние заимодействие с лестницей.
*/
bool& APlayer::GetIsOnLadder()
{
    return bIsOnLadder;
}

bool APlayer::GetIsDeathPlayer() const
{
    return bIsDeath;
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
        const float SpawnBulletOffsetX = bIsMoveRight ? 12.f : 0.f;
        constexpr float SpawnBulletOffsetY = 11.f;
        const sf::IntRect BulletRectTexture = sf::IntRect(7, 72, static_cast<int>(BULLET_SIZE.x),
                                                          static_cast<int>(BULLET_SIZE.y));

        // Создание нового снаряда и добавление его в вектор снарядов
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, BulletRectTexture, EBulletType::EBT_ShootAtEnemy,
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

    if (!bIsDeath)
    {
        // Движение влево
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            bIsMoveRight = false;
            ActorVelocity.x = -PawnSpeed * DeltaTime;
            ActorSprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        // Движение вправо
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            bIsMoveRight = true;
            ActorVelocity.x = PawnSpeed * DeltaTime;
            ActorSprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
        }
        // Движение вверх (подъем по лестнице)
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            if (bIsOnLadder)
            {
                ActorVelocity.y = PawnSpeed * DeltaTime;
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
            if (bIsPawnJump)
            {
                bIsPawnJump = false;
                ActorVelocity.y = PlayerJumpSpeed * DeltaTime;
            }
        }
    }
}

/**
 * @brief Обновление позиции и состояния персонажа.
 * Включает в себя обработку гравитации и проверку столкновений с объектами.
 * 
 * @param DeltaTime Время, прошедшее с предыдущего кадра.
 */
void APlayer::UpdatePlayerMove(float DeltaTime)
{
    // Применение гравитации для падения персонажа
    ActorVelocity.y += GRAVITY * DeltaTime;

    // Обновление позиции персонажа на основе скорости
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;

    // Обновление позиции спрайта персонажа и его коллизии
    ActorDrawPosition = {
        ActorCollisionRect.left + (PLAYER_SIZE.x * DRAW_SCALE.x) / 2.f,
        ActorCollisionRect.top + (PLAYER_SIZE.y * DRAW_SCALE.y) / 2.f
    };

    // TODO: Test Animation
    if (bIsPawnJump)
    {
        if (std::abs(ActorVelocity.x) > 0.0001f)
        {
            CurrentAnimation = &WalkAnimation;
        }
        else
        {
            CurrentAnimation = &IdleAnimation;
        }
    }
    else
    {
        if (ActorVelocity.y < 0.f)
        {
            CurrentAnimation = &JumpDownAnimation;
        }
        else
        {
            CurrentAnimation = &JumpUpAnimation;
        }
    }

    // Если здоровье меньше или равно 0, то проигрываем анимацию смерти и меняем флаг на "Умер"
    if (PawnCurrentHealth <= DEATH)
    {
        bIsDeath = true;
        CurrentAnimation = &DeathAnimation;
    }

    // Если персонаж мёрт, то от текущего здоровья отнимаем текущее,
    // чтобы всегда шкала здоровья при смерте равнялась 0
    if (bIsDeath)
    {
        // Устанавливаем здоровье персонажу
        SetPawnCurrentHealth(GetPawnCurrentHealth());
    }

    ActorSprite.setTextureRect(CurrentAnimation->GetCurrentFrame());
    CurrentAnimation->AnimationUpdate(DeltaTime);

    // Установка позиции спрайта персонажа
    ActorSprite.setPosition(ActorDrawPosition);
}

void APlayer::SetIsDeathPlayer(bool IsDeath)
{
    bIsDeath = IsDeath;
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
    Window.draw(ActorSprite);

    // Отрисовка шкалы здоровья
    UpdatePawnHealthBar(PawnCurrentHealth, PawnMaxHealth,
                        sf::Vector2f(ActorDrawPosition.x - 0.f, ActorDrawPosition.y - 10.f),
                        sf::Vector2f(ActorDrawPosition.x - 760.f, ActorDrawPosition.y - 380.f));

    Window.draw(HealthBarSprite);
    PawnHealthBarPtr->Draw(Window);
}
