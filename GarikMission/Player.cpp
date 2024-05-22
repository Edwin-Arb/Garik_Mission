#include "Player.h"

APlayer::APlayer()
    : isPlayerOnGround(false),
      playerSpeed(PAWN_SPEED),
      playerJumpSpeed(1000.f),
      playerVelocity({0.f, 0.f}),
      playerRect({100.f, 400.f,
                   PLAYER_SIZE.x * DRAW_SCALE.x,
                   PLAYER_SIZE.y * DRAW_SCALE.y}),
      playerShapeCollision(sf::Vector2f(PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.y))
{
}

void APlayer::InitPlayer()
{
    // Подгрузить текстуру из папки для персонажа
    assert(playerTexture.loadFromFile(RESOURCES_PATH + "MainTiles/player.png"));

    // Создать спрайт персонажа и положение на карте
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(19, 0,
                                            static_cast<int>(PLAYER_SIZE.x),
                                            static_cast<int>(PLAYER_SIZE.y)));
    playerSprite.setScale(DRAW_SCALE);
}

void APlayer::UpdatePlayerMove(const sf::Event& event, float delta_time)
{
    // Передвижение
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        playerVelocity.x = -playerSpeed * delta_time;
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerVelocity.x = playerSpeed * delta_time;
    }

    // Прыжок
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if (isPlayerOnGround)
        {
            isPlayerOnGround = false;
            playerVelocity.y = playerJumpSpeed * delta_time;
        }
    }

    // Гравитация необходима для приземления персонажа на землю
    playerVelocity.y += GRAVITY * delta_time;

    playerRect.left += playerVelocity.x;
    playerRect.top -= playerVelocity.y;

    // Проверка коллизии с землёй карты
    const float playerRectBottom = playerRect.top + playerRect.height;
    if (playerRectBottom > MAP_SURFACE)
    {
        isPlayerOnGround = true;
        playerVelocity.y = 0.f;
        playerRect.top = MAP_SURFACE - playerRect.height;
    }

    // Проверка коллизии с правой частью экрана
    const float playerRectTop = playerRect.left + playerRect.width;
    if (playerRectTop > SCREEN_WIDTH)
    {
        playerVelocity.x = 0.f;
        playerRect.left = SCREEN_WIDTH - playerRect.width;
    }

    // Проверка коллизии с левой частью экрана
    if (playerRect.left < 0.f)
    {
        playerVelocity.x = 0.f;
        playerRect.left = 0.f;
    }

    playerVelocity.x = 0.f;    
}

sf::FloatRect APlayer::GetPlayerRect() const
{// Получаем коллизию персонажа
    return playerRect;
}

void APlayer::DrawPlayer(sf::RenderWindow& window)
{
    // Установить положение спрайта с положением персонажа в игре, т.е с его прямоугольником(коллизией)
    playerSprite.setPosition(playerRect.left, playerRect.top);
    playerShapeCollision.setPosition(playerRect.left, playerRect.top);

    // Рисовать персонажа и его коллизию
    window.draw(playerShapeCollision);
    window.draw(playerSprite);
}
