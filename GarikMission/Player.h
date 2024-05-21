#pragma once
#include "Constants.h"

class APlayer
{
public:
    APlayer();
    
    void InitPlayer();
    void UpdatePlayerMove(float deltaTime);
    //void SetPlayerDirection(EPawnDirection newDirection);
    void SetPlayerVelocity(sf::Vector2f newVelocity);
    void SetPlayerSpeed(float newSpeed);
    void DrawPlayer(sf::RenderWindow& window);
    Rectangle GetPlayerCollider() const;

private:
    bool isPlayerOnGround;
    float playerSpeed;
    float playerJumpSpeed;
    
    //EPawnDirection direction;
    
    sf::Vector2f playerVelocity;
    sf::FloatRect playerRect;

    sf::Texture playerTexture;
    sf::Sprite playerSprite;
};
