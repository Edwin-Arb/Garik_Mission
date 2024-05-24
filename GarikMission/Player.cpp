#include "Player.h"
#include "GameEngine.h"

APlayer::APlayer()
    : bIsPlayerOnGround(false),
      PlayerSpeed(PAWN_SPEED),
      PlayerJumpSpeed(1000.f),
      PlayerVelocity({0.f, 0.f}),
      PlayerRect({100.f, 400.f,
                   PLAYER_SIZE.x * DRAW_SCALE.x,
                   PLAYER_SIZE.y * DRAW_SCALE.y}),
      PlayerTexturePtr(new sf::Texture),
      PlayerRectCollision(sf::Vector2f(PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y))
{}

APlayer::~APlayer()
{
    delete PlayerTexturePtr;
}

void APlayer::InitPlayer()
{
    // Подгрузить текстуру из папки для персонажа
    assert(PlayerTexturePtr->loadFromFile(RESOURCES_PATH + "MainTiles/Player.png"));

    // Создать спрайт персонажа и установить его центр
    PlayerSprite.setTexture(*PlayerTexturePtr);
    PlayerSprite.setTextureRect(sf::IntRect(19, 0,
                                            static_cast<int>(PLAYER_SIZE.x),
                                            static_cast<int>(PLAYER_SIZE.y)));
    
    AGameEngine::SetSpriteSize(PlayerSprite, PLAYER_SIZE.x * DRAW_SCALE.x,PLAYER_SIZE.y * DRAW_SCALE.y);
    AGameEngine::SetSpriteRelativeOrigin(PlayerSprite, 0.5f, 0.5f);

    // Создать спрайт коллизии и установить его центр
    AGameEngine::SetShapeRelativeOrigin(PlayerRectCollision, 0.5f, 0.5f);
}

void APlayer::UpdatePlayerMove(float DeltaTime)
{
    // Передвижение
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        PlayerVelocity.x = -PlayerSpeed * DeltaTime;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        PlayerVelocity.x = PlayerSpeed * DeltaTime;
    }

    // Прыжок
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (bIsPlayerOnGround)
        {
            bIsPlayerOnGround = false;
            PlayerVelocity.y = PlayerJumpSpeed * DeltaTime;
        }
    }

    // Гравитация необходима для приземления персонажа на землю
    PlayerVelocity.y += GRAVITY * DeltaTime;

    PlayerRect.left += PlayerVelocity.x;
    PlayerRect.top -= PlayerVelocity.y;

    // Проверка коллизии с землёй карты
    const float PlayerRectBottom = PlayerRect.top + PlayerRect.height;
    if (PlayerRectBottom > MAP_SURFACE)
    {
        bIsPlayerOnGround = true;
        PlayerVelocity.y = 0.f;
        PlayerRect.top = MAP_SURFACE - PlayerRect.height;
    }

    // Проверка коллизии с правой частью экрана
    const float PlayerRectTop = PlayerRect.left + PlayerRect.width;
    if (PlayerRectTop > SCREEN_WIDTH)
    {
        PlayerVelocity.x = 0.f;
        PlayerRect.left = SCREEN_WIDTH - PlayerRect.width;
    }

    // Проверка коллизии с левой частью экрана
    if (PlayerRect.left < 0.f)
    {
        PlayerVelocity.x = 0.f;
        PlayerRect.left = 0.f;
    }

    PlayerVelocity.x = 0.f;    
}

sf::FloatRect APlayer::GetPlayerRect() const
{// Получаем коллизию персонажа
    return PlayerRect;
}

void APlayer::DrawPlayer(sf::RenderWindow& Window)
{
    // Установить положение спрайта с положением персонажа в игре, т.е с его прямоугольником(коллизией)
    PlayerSprite.setPosition(PlayerRect.left, PlayerRect.top);
    PlayerRectCollision.setPosition(PlayerRect.left, PlayerRect.top);

    // Рисовать персонажа и его коллизию
    Window.draw(PlayerRectCollision);
    Window.draw(PlayerSprite);
}
