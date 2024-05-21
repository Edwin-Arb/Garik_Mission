#include "Player.h"

APlayer::APlayer()
{
    isPlayerOnGround = false;
    playerSpeed = SPEED_PAWN;
    playerJumpSpeed = 1000.f;
    
    // = EPawnDirection::EPD_StartPosition;
    
    playerVelocity = {0.f, 0.f};
    
    playerRect = {100.f, 400.f,
                   PLAYER_SIZE.x * DRAW_SCALE.x, PLAYER_SIZE.y * DRAW_SCALE.x};
}

void APlayer::InitPlayer()
{
    // Подгрузить текстуру из папки для персонажа
    playerTexture.loadFromFile(RESOURCES_PATH + "MainTiles/player.png");
    
    // Создать спрайт персонажа и положение на карте
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(19, 0,
                                static_cast<int>(PLAYER_SIZE.x),
                                static_cast<int>(PLAYER_SIZE.y)));
    playerSprite.setScale(DRAW_SCALE);
}

void APlayer::UpdatePlayerMove(float deltaTime)
{
    // if(sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    // {
    //     playerVelocity.y = -playerSpeed * deltaTime;
    // }
    // else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    // {
    //     if(isPlayerOnGround)
    //     {
    //         playerVelocity.y = playerSpeed * deltaTime;
    //     }
    // }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        playerVelocity.x = -playerSpeed * deltaTime;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        playerVelocity.x = playerSpeed * deltaTime;
    }
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        if(isPlayerOnGround)
        {
            isPlayerOnGround = false;
            playerVelocity.y = playerJumpSpeed * deltaTime;
        }
    }

    {
        // Гравитация необходима для приземления персонажа на землю
        playerVelocity.y += GRAVITY * deltaTime;
        
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
}

// void APlayer::SetPlayerDirection(EPawnDirection newDirection)
// {
//     // Установить направление
//     direction = newDirection;
// }

void APlayer::SetPlayerVelocity(sf::Vector2f newVelocity)
{
    // Установить позицию
    playerVelocity = newVelocity;
}

void APlayer::SetPlayerSpeed(float newSpeed)
{
    // Установить скорость
    playerVelocity.x = newSpeed;
    playerVelocity.y = newSpeed;
}

void APlayer::DrawPlayer(sf::RenderWindow &window)
{
    // Задать одинаковую позицию спрайту и положению персонажа
    playerSprite.setPosition(playerRect.left, playerRect.top);
    window.draw(playerSprite);
}

Rectangle APlayer::GetPlayerCollider() const
{
    // Координаты верхнего левого угла спрайта персонажа
    sf::Vector2f topLeft(playerRect.left, playerRect.top);
    
    // Координаты нижнего правого угла спрайта персонажа
    sf::Vector2f bottomRight(playerRect.width + PLAYER_SIZE.x * DRAW_SCALE.x, 
                             playerRect.height + PLAYER_SIZE.y * DRAW_SCALE.x);
    
    return { {topLeft.x, topLeft.y}, {bottomRight.x, bottomRight.y} };
}
