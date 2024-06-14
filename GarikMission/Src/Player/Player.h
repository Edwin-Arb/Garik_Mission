#pragma once
#include "../Bullet/Bullet.h"
#include "../UserInterface/HealthBar.h"

// Предварительное объявление (Forward declaration of ACollisionManager)
class ACollisionManager;

/**
 * Класс игрока, отвечающий за поведение и состояние персонажа.
 */
class APlayer
{
public:
    APlayer();   ///< Конструктор
    ~APlayer();  ///< Деструктор
    
    // Инициализация персонажа
    void InitPlayer(ASpriteManager& SpriteManager);

    // Обработка выстрелов игрока
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& RendererSprite) const;

    // Обработка движения игрока
    void HandlePlayerMove(float DeltaTime);

    // Обновление движения игрока
    void UpdatePlayerMove(float DeltaTime, const ACollisionManager& CollisionManager);
    
private:
    // Обновление отображения шкалы здоровья игрока
    void UpdatePlayerHealthBar();

public:
    // Установка скорости игрока
    void SetPlayerVelocity(const sf::Vector2f& NewVelocity);

    // Установка прямоугольника персонажа
    void SetPlayerRect(const sf::FloatRect& NewRect);

    // Установка максимального здоровья игрока
    void SetPlayerMaxHealth(int NewPlayerHealth);
    
    // Получение максимального здоровья игрока
    float GetPlayerMaxHealth() const;
    
    // Получение текущей позиции игрока
    sf::Vector2f GetPlayerPossition() const;

    // Получение прямоугольника персонажа
    sf::FloatRect GetPlayerRect() const;

    // Отрисовка игрока на экране
    void DrawPlayer(sf::RenderWindow& Window);

private:
    bool bCanJump;                       // Флаг, показывающий, может ли игрок прыгать
    bool bIsMoveRight;                   // Флаг, указывающий направление движения игрока
    bool bIsOnLadder;                    // Флаг, указывающий, находится ли игрок на лестнице
    float PlayerHealth;                  // Текущее здоровье игрока
    float PlayerSpeed;                   // Скорость перемещения игрока
    float PlayerJumpSpeed;               // Скорость прыжка игрока
    
    sf::Vector2f PlayerVelocity;         // Текущая скорость и направление движения игрока
    sf::FloatRect PlayerRect;            // Прямоугольник, ограничивающий персонажа
    sf::Vector2f PlayerDrawPosition;     // Позиция для отрисовки игрока
    
    sf::Texture* PlayerTexturePtr;       // Указатель на текстуру персонажа
    sf::Sprite PlayerSprite;             // Спрайт персонажа
    
    AHealthBar* HealthBarPtr;            // Указатель на шкалу здоровья игрока
    sf::Texture* HealthBarTexturePtr;    // Текстура шкалы здоровья
    sf::Sprite HealthBarSprite;          // Спрайт шкалы здоровья

    // TODO: Для тестирования коллизий, удалить позже
    sf::RectangleShape PlayerRectCollision;  // Прямоугольная область для коллизий игрока
};