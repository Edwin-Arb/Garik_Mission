#pragma once
#include "../Constants.h"
#include "../Renderer/IRendererSprite.h"


class APlayer
{
public:
    APlayer();
    ~APlayer();

    void InitPlayer(IRendererSprite &RendererSprite);
    void UpdatePlayerMove(float DeltaTime);
    sf::FloatRect GetPlayerRect() const;
    void DrawPlayer(sf::RenderWindow& Window);

private:
    // Параметры персонажа
    bool bIsPlayerOnGround;
    float PlayerSpeed;
    float PlayerJumpSpeed;

    // Направление и коллизия
    sf::Vector2f PlayerVelocity;
    sf::FloatRect PlayerRect;

    // Текстура персонажа
    sf::Texture *PlayerTexturePtr;
    sf::Sprite PlayerSprite;
    
    // для тестов коллизии, потом удалить
    sf::RectangleShape PlayerRectCollision;
};
