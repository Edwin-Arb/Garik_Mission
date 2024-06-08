#include "Player.h"
#include "../Manager/CollisionManager.h"

class ACollisionManager;

APlayer::APlayer()
    : bCanJump(true)
      , bIsMoveRight(true)
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
    HealthBarPtr->InitHealthBar(100, 25, sf::Color::Red, sf::Color::Yellow, SpriteManager);
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

void APlayer::SetPlayerVelocity(const sf::Vector2f& NewVelocity)
{
    PlayerVelocity = NewVelocity;
}

void APlayer::SetPlayerRect(const sf::FloatRect& NewRect)
{
    PlayerRect = NewRect;
}

void APlayer::HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& RendererSprite) const
{
    // Выстрел из оружия. Пока кнопка нажата - мы стреляем 
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        const float SpawnBulletOffsetX = bIsMoveRight ? 90.f : 0.f;
        constexpr float SpawnBulletOffsetY = 85.f;
        BulletsVectorPtr.emplace_back(new ABullet(bIsMoveRight,
                                                  sf::Vector2f(PlayerRect.left + SpawnBulletOffsetX,
                                                               PlayerRect.top + SpawnBulletOffsetY),
                                                  RendererSprite));
    }
}

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
        PlayerVelocity.y = PlayerSpeed * DeltaTime;
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
}


void APlayer::UpdatePlayerHealthBar()
{
    HealthBarPtr->Update(200, 400);
    HealthBarPtr->SetPosition({PlayerDrawPosition.x - 300.f, PlayerDrawPosition.y - 300.f});
    HealthBarSprite.setPosition(PlayerDrawPosition.x - 760.f, PlayerDrawPosition.y - 380.f);
}

sf::Vector2f APlayer::GetPlayerPossition() const
{
    return PlayerDrawPosition;
}

sf::FloatRect APlayer::GetPlayerRect() const
{
    // Получаем коллизию персонажа
    return PlayerRect;
}

void APlayer::DrawPlayer(sf::RenderWindow& Window)
{
    // Установить положение спрайта с положением персонажа в игре, т.е с его прямоугольником(коллизией)
    PlayerSprite.setPosition(PlayerDrawPosition);
    PlayerRectCollision.setPosition(PlayerDrawPosition);

    // Рисовать персонажа и его коллизию
    Window.draw(PlayerRectCollision);
    Window.draw(PlayerSprite);

    // Рисовать шкалу здоровья 
    UpdatePlayerHealthBar();
    HealthBarPtr->Draw(Window);
    Window.draw(HealthBarSprite);
}
