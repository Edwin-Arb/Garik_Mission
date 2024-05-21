#pragma once
#include "Constants.h"
#include "Bullet.h"

class APlayer
{
public:
    APlayer();
    
    void InitPlayer();
    void UpdatePlayerMove(const sf::Event& event, float deltaTime);
    void PlayerShoot(const sf::Event& event, float deltaTime);
    //void SetPlayerDirection(EPawnDirection newDirection);
    void SetPlayerVelocity(sf::Vector2f newVelocity);
    void SetPlayerSpeed(float newSpeed);
    void DrawPlayer(sf::RenderWindow& window);
    Rectangle GetPlayerCollider() const;

private:
    bool TEST_BULLET = false;
    bool isPlayerOnGround;
    float playerSpeed;
    float playerJumpSpeed;
    ABullet *bulletPtr;
    //EPawnDirection direction;
    
    sf::Vector2f playerVelocity;
    sf::FloatRect playerRect;

    sf::Texture playerTexture;
    sf::Sprite playerSprite;

    // для тестов коллизии, потом удалить
    sf::RectangleShape playerShapeCollision;
};
