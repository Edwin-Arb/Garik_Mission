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
      , PlayerJumpSpeed(1500.f)
{
    // Максимальное и текущие здоровья персонажа,
    // чтобы использовать для ослеживания шкалы здоровья.
    PawnMaxHealth = 400.f;
    PawnCurrentHealth = PawnMaxHealth;

    // Скорость персонажа
    PawnSpeed = PAWN_SPEED;

    // Скорость и направление персонажа
    ActorVelocity = {0.f, 0.f};

    // Прямоугольник персонажа с учетом масштабирования
    ActorCollisionRect = {
        100.f, 400.f,
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
    // Инициализировать текстуру для персонажа и создать спрайт для него
    InitActorTexture(ASSETS_PATH + "MainTiles/Player.png",
                     sf::IntRect(19, 0, static_cast<int>(PLAYER_SIZE.x), static_cast<int>(PLAYER_SIZE.y)),
                     {PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y},
                     {0.5f, 0.5f},
                     SpriteManager);

    // Инициализация шкалы здоровья
    InitPawnHealthBar(ASSETS_PATH + "MainTiles/HealthBarPlayer.png",
                      sf::Vector2f(165.f, 25.f), sf::Vector2f(0.2f, 0.2f),
                      sf::Color::Red, sf::Color::Yellow,
                      SpriteManager);
}

/**
 * @brief Получить по ссылки данные о состояние прыжка.
 * 
 * @return Текущие состояние прыжка.
 */
bool& APlayer::GetCanJump()
{
    return bCanJump;
}

/**
* @brief Получить по ссылки данные, если персонаж находить рядом с лестницей и может карабкаться по ней.
* 
* @return Текущие состояние заимодействие с лестницей.
*/
bool& APlayer::GetIsOnLadder()
{
    return bIsOnLadder;
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

    // Установка позиции спрайта персонажа
    ActorSprite.setPosition(ActorDrawPosition);
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
                        sf::Vector2f(ActorDrawPosition.x - 635.f, ActorDrawPosition.y - 360.f),
                        sf::Vector2f(ActorDrawPosition.x - 760.f, ActorDrawPosition.y - 380.f));

    Window.draw(HealthBarSprite);
    PawnHealthBarPtr->Draw(Window);
}
