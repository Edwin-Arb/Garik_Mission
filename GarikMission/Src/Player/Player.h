#pragma once
#include "../Bullet/Bullet.h"
#include "../UserInterface/HealthBar.h"


class APlayer
{
public:
    APlayer();
    ~APlayer();

    void InitPlayer(ASpriteManager& RendererSprite);
    void SetPlayerVelocity(const sf::Vector2f& NewVelocity);
    void SetPlayerRect(const sf::FloatRect& NewRect);
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const;
    void HandlePlayerMove(float DeltaTime);
    void UpdatePlayerMove(float DeltaTime, const class ACollisionManager& CollisionManager);
    void UpdatePlayerHealthBar();
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

    // Шкала здоровья и текструа
    AHealthBar *HealthBarPtr;    
    sf::Texture *HealthBarTexturePtr;
    sf::Sprite HealthBarSprite;

    // TODO для тестов коллизии, потом удалить
    sf::RectangleShape PlayerRectCollision;
};
