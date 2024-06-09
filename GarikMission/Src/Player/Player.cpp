#include "Player.h"
#include "../Manager/CollisionManager.h"

class ACollisionManager;

APlayer::APlayer()
    : bCanJump(true)
      , bIsMoveRight(true)
      , PlayerHealth(400)
      , PlayerSpeed(PAWN_SPEED)
      , PlayerJumpSpeed(1500.f)
      , PlayerVelocity({0.f, 0.f})
      , PlayerRect({
          100.f, 400.f,
          PLAYER_SIZE.x * DRAW_SCALE.x,
          PLAYER_SIZE.y * DRAW_SCALE.y
      })
      , PlayerTexturePtr(new sf::Texture)
      , HealthBarTexturePtr(new sf::Texture)
      , HealthBarPtr(new AHealthBar)
      , PlayerRectCollision(sf::Vector2f(PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y))
{
}

APlayer::~APlayer()
{
    delete PlayerTexturePtr;
    delete HealthBarPtr;
    delete HealthBarTexturePtr;
}

void APlayer::InitPlayer(ASpriteManager& SpriteManager)
{
    // Подгрузить текстуры из папки для персонажа
    assert(PlayerTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/Player.png"));
    assert(HealthBarTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/HealthBarPlayer.png"));

    // Инициализация шкалы здоровья
    HealthBarPtr->InitHealthBar(165, 25, sf::Color::Red, sf::Color::Yellow, SpriteManager);
    HealthBarSprite.setTexture(*HealthBarTexturePtr);
    HealthBarSprite.setScale(0.2f, 0.2f);

    // Создать спрайт персонажа и установить его центр
    PlayerSprite.setTexture(*PlayerTexturePtr);
    PlayerSprite.setTextureRect(sf::IntRect(19, 0,
                                            static_cast<int>(PLAYER_SIZE.x),
                                            static_cast<int>(PLAYER_SIZE.y)));

    // Установить масштаб спрайта персонажа
    SpriteManager.SetSpriteSize(PlayerSprite, PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y);

    // Установить центр спрайта персонажа
    SpriteManager.SetSpriteRelativeOrigin(PlayerSprite, 0.5f, 0.5f);

    // Создать спрайт коллизии и установить его центр
    SpriteManager.SetShapeRelativeOrigin(PlayerRectCollision, 0.5f, 0.5f);
}

// Обработка выстрелов игрока
void APlayer::HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const
{
    // Выстрел из оружия. Пока кнопка нажата - мы стреляем 
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? 90.f : 0.f;
        constexpr float SpawnBulletOffsetY = 85.f;
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight, EBulletType::EBT_ShootAtEnemy,
                                                  sf::Vector2f(PlayerRect.left + SpawnBulletOffsetX,
                                                               PlayerRect.top + SpawnBulletOffsetY),
                                                  SpriteManager));
    }
}

// Обработка передвижения игрока
void APlayer::HandlePlayerMove(float DeltaTime)
{
    // Установить направление передвижения персонажа
    PlayerVelocity.x = 0.f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        // Влево
        bIsMoveRight = false;
        PlayerVelocity.x = -PlayerSpeed * DeltaTime;
        PlayerSprite.setScale(-1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        // Вправо
        bIsMoveRight = true;
        PlayerVelocity.x = PlayerSpeed * DeltaTime;
        PlayerSprite.setScale(1.f * DRAW_SCALE.x, 1.f * DRAW_SCALE.y);
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        // Вверх
        PlayerVelocity.y = 2;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        // Вниз
        PlayerVelocity.y = -PlayerSpeed * DeltaTime;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        // Прыжок
        if (bCanJump)
        {
            bCanJump = false;
            PlayerVelocity.y = PlayerJumpSpeed * DeltaTime;
        }
    }
}

// Обновление передвижения игрока
void APlayer::UpdatePlayerMove(float DeltaTime, const ACollisionManager& CollisionManager)
{
    // Гравитация необходима для приземления персонажа на землю
    PlayerVelocity.y += GRAVITY * DeltaTime;

    PlayerRect.left += PlayerVelocity.x;
    PlayerRect.top -= PlayerVelocity.y;

    // Проверка столкновений персонажа с объектами карты
    CollisionManager.HandlePlayerCollisionWithGameMap(PlayerRect, PlayerVelocity, bCanJump);

    // Обновление позиции спрайта и формы коллизии
    PlayerDrawPosition =
    {
        PlayerRect.left + (PLAYER_SIZE.x * DRAW_SCALE.x) / 2.f,
        PlayerRect.top + (PLAYER_SIZE.y * DRAW_SCALE.y) / 2.f
    };

    // Установить положение спрайта с положением персонажа в игре, т.е с его прямоугольником(коллизией)
    PlayerSprite.setPosition(PlayerDrawPosition);
    PlayerRectCollision.setPosition(PlayerDrawPosition);
}

// Обновление шкалы здоровья игрока
void APlayer::UpdatePlayerHealthBar()
{
    HealthBarPtr->Update(PlayerHealth, 400);
    HealthBarPtr->SetPosition({PlayerDrawPosition.x - 635.f, PlayerDrawPosition.y - 360.f});
    HealthBarSprite.setPosition(PlayerDrawPosition.x - 760.f, PlayerDrawPosition.y - 380.f);

    // TODO Восстановление здоровья для тестов
    if (PlayerHealth <= DEATH)
    {
        PlayerHealth = 400;
    }
}

// Установка скорости игрока
void APlayer::SetPlayerVelocity(const sf::Vector2f& NewVelocity)
{
    PlayerVelocity = NewVelocity;
}

// Установка прямоугольника игрока
void APlayer::SetPlayerRect(const sf::FloatRect& NewRect)
{
    PlayerRect = NewRect;
}

void APlayer::SetPlayerMaxHealth(int NewPlayerHealth)
{
    PlayerHealth -= NewPlayerHealth;
}

int APlayer::GetPlayerMaxHealth() const
{
    return PlayerHealth;
}

// Получение позиции игрока
sf::Vector2f APlayer::GetPlayerPossition() const
{
    return PlayerDrawPosition;
}

// Получение прямоугольника(коллизии) игрока
sf::FloatRect APlayer::GetPlayerRect() const
{
    return PlayerRect;
}

// Отрисовка игрока
void APlayer::DrawPlayer(sf::RenderWindow& Window)
{
    // Рисовать персонажа и его коллизию
    Window.draw(PlayerRectCollision);
    Window.draw(PlayerSprite);

    // Рисовать шкалу здоровья 
    UpdatePlayerHealthBar();
    Window.draw(HealthBarSprite);
    HealthBarPtr->Draw(Window);
}
