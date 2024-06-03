#include "Player.h"
#include "../GameMap/GameMap.h"

class ACollisionManager;

APlayer::APlayer()
    :   bCanJump(true)
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
      , PlayerRectCollision(sf::Vector2f(PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y))
{
}

APlayer::~APlayer()
{
    delete PlayerTexturePtr;
}

void APlayer::InitPlayer(ASpriteManager& RendererSprite)
{
    // Подгрузить текстуру из папки для персонажа
    assert(PlayerTexturePtr->loadFromFile(ASSETS_PATH + "MainTiles/Player.png"));

    // Создать спрайт персонажа и установить его центр
    PlayerSprite.setTexture(*PlayerTexturePtr);
    PlayerSprite.setTextureRect(sf::IntRect(19, 0,
                                            static_cast<int>(PLAYER_SIZE.x),
                                            static_cast<int>(PLAYER_SIZE.y)));

    // Установить масштаб спрайта персонажа
    RendererSprite.SetSpriteSize(PlayerSprite, PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y);

    // Установить центр спрайта персонажа
    RendererSprite.SetSpriteRelativeOrigin(PlayerSprite, 0.5f, 0.5f);

    // Создать спрайт коллизии и установить его центр
    RendererSprite.SetShapeRelativeOrigin(PlayerRectCollision, 0.5f, 0.5f);
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

void APlayer::UpdatePlayerMove(float DeltaTime, const AGameMap& GameMap)
{
    // Гравитация необходима для приземления персонажа на землю
    PlayerVelocity.y += GRAVITY * DeltaTime;

    PlayerRect.left += PlayerVelocity.x;
    PlayerRect.top -= PlayerVelocity.y;

    for (const auto& Obstacle : GameMap.GetCollisionVector())
    {
        if (PlayerRect.intersects(Obstacle))
        {
            HandleCollision(Obstacle);
        }
    }

    // Обновление позиции спрайта и формы коллизии
    PlayerDrawPosition =
    {
        PlayerRect.left + (PLAYER_SIZE.x * DRAW_SCALE.x) / 2.f,
        PlayerRect.top + (PLAYER_SIZE.y * DRAW_SCALE.y) / 2.f
    };
}

sf::FloatRect APlayer::GetPlayerRect() const
{
    // Получаем коллизию персонажа
    return PlayerRect;
}

void APlayer::HandleCollision(const sf::FloatRect& Obstacle)
{
    // Проверяем пересечение игрока и препятствия
    if (PlayerRect.intersects(Obstacle))
    {
        float OverlapLeft = (PlayerRect.left + PlayerRect.width) - Obstacle.left;
        float OverlapRight = (Obstacle.left + Obstacle.width) - PlayerRect.left;
        float OverlapTop = (PlayerRect.top + PlayerRect.height) - Obstacle.top;
        float OverlapBottom = (Obstacle.top + Obstacle.height) - PlayerRect.top;

        bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
        bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

        float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
        float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

        if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
        {
            // Горизонтальное столкновение
            PlayerRect.left += FromLeft ? -OverlapLeft : OverlapRight;

            // Останавливаем горизонтальное движение
            PlayerVelocity.x = 0.f;
        }
        else
        {
            // Вертикальное столкновение
            PlayerRect.top += FromTop ? -OverlapTop : OverlapBottom;

            // Останавливаем вертикальное движение
            PlayerVelocity.y = 0.f;

            // Если мы на земле, то даём прыгать персонажу 
            bCanJump = true;
        }
    }
}

void APlayer::DrawPlayer(sf::RenderWindow& Window)
{
    // Установить положение спрайта с положением персонажа в игре, т.е с его прямоугольником(коллизией)
    PlayerSprite.setPosition(PlayerDrawPosition);
    PlayerRectCollision.setPosition(PlayerDrawPosition);

    // Рисовать персонажа и его коллизию
    Window.draw(PlayerRectCollision);
    Window.draw(PlayerSprite);
}
