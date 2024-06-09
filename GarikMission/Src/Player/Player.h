#pragma once
#include "../Bullet/Bullet.h"
#include "../UserInterface/HealthBar.h"

// Forward declaration of ACollisionManager
class ACollisionManager;

class APlayer
{
public:
    APlayer();
    ~APlayer();
    
    // Инициализация персонажа
    void InitPlayer(ASpriteManager& SpriteManager);

    // Обработка выстрелов персонажа
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& RendererSprite) const;

    // Обработка передвижения персонажа
    void HandlePlayerMove(float DeltaTime);

    // Обновление передвижения персонажа
    void UpdatePlayerMove(float DeltaTime, const ACollisionManager& SpriteManager);

    // Обновление шкалы здоровья персонажа
    void UpdatePlayerHealthBar();
    
    // Установка скорости персонажа
    void SetPlayerVelocity(const sf::Vector2f& NewVelocity);

    // Установка прямоугольника персонажа
    void SetPlayerRect(const sf::FloatRect& NewRect);

    // Установка
    void SetPlayerMaxHealth(int NewPlayerHealth);
    
    // Получения здоровья 
    int GetPlayerMaxHealth() const;
    
    // Получение позиции персонажа
    sf::Vector2f GetPlayerPossition() const;

    // Получение прямоугольника персонажа
    sf::FloatRect GetPlayerRect() const;

    // Отрисовка персонажа
    void DrawPlayer(sf::RenderWindow& Window);

private:
    bool bCanJump;                       // Может ли игрок прыгать
    bool bIsMoveRight;                   // Направление движения персонажа
    int PlayerHealth;                    // Максимальное здоровье персонажа
    float PlayerSpeed;                   // Скорость персонажа
    float PlayerJumpSpeed;               // Скорость прыжка персонажа
    
    sf::Vector2f PlayerVelocity;         // Скорость персонажа и направления движения
    sf::FloatRect PlayerRect;            // Прямоугольник персонажа
    sf::Vector2f PlayerDrawPosition;     // Позиция для отрисовки персонажа
    
    sf::Texture* PlayerTexturePtr;       // Текстура персонажа
    sf::Sprite PlayerSprite;             // Спрайт персонажа
    
    AHealthBar* HealthBarPtr;            // Указатель на шкалу здоровья
    sf::Texture* HealthBarTexturePtr;    // Текстура шкалы здоровья
    sf::Sprite HealthBarSprite;          // Спрайт шкалы здоровья

    // TODO для тестов коллизии, потом удалить
    sf::RectangleShape PlayerRectCollision;  // Прямоугольник для коллизии персонажа
};
