#pragma once
#include "../Bullet/Bullet.h"



class APlayer
{
public:
    APlayer();
    ~APlayer();

    void InitPlayer(ASpriteManager &RendererSprite);
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& RendererSprite) const;
    void HandlePlayerMove(float DeltaTime);
    void UpdatePlayerMove(float DeltaTime);
    sf::FloatRect GetPlayerRect() const;
    void DrawPlayer(sf::RenderWindow& Window);

private:
    // Параметры персонажа
    bool bIsPlayerOnGround;
    float PlayerSpeed;
    float PlayerJumpSpeed;
        
    // Движение и коллизия
    sf::Vector2f PlayerVelocity;
    sf::FloatRect PlayerRect;

    // Текстура персонажа
    sf::Texture *PlayerTexturePtr;
    sf::Sprite PlayerSprite;
    
    // для тестов коллизии, потом удалить
    sf::RectangleShape PlayerRectCollision;
};
