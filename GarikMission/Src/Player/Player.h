#pragma once
#include "../Bullet/Bullet.h"


class APlayer
{
public:
    APlayer();
    ~APlayer();

    void InitPlayer(ASpriteManager& RendererSprite);
    void SetPlayerVelocity(const sf::Vector2f& NewVelocity);
    void SetPlayerRect(const sf::FloatRect& NewRect);
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& RendererSprite) const;
    void HandlePlayerMove(float DeltaTime);
    void UpdatePlayerMove(float DeltaTime, const class ACollisionManager& CollisionManager);
    sf::Vector2f GetPlayerPossition() const;
    sf::FloatRect GetPlayerRect() const;
    
    void DrawPlayer(sf::RenderWindow& Window);

private:
    // Параметры персонажа
    bool bCanJump;
    bool bIsMoveRight;
    float PlayerSpeed;
    float PlayerJumpSpeed;

    // Движение и коллизия
    sf::Vector2f PlayerVelocity;
    sf::FloatRect PlayerRect;
    sf::Vector2f PlayerDrawPosition;

    // Текстура персонажа
    sf::Texture* PlayerTexturePtr;
    sf::Sprite PlayerSprite;

    // для тестов коллизии, потом удалить
    sf::RectangleShape PlayerRectCollision;
};
