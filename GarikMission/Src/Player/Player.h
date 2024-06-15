#pragma once
#include "../Abstract/AActor.h"
#include "../Bullet/Bullet.h"
#include "../UserInterface/HealthBar.h"

// Предварительное объявление (Forward declaration of ACollisionManager)
class ACollisionManager;

/**
 * @brief Класс персонажа, отвечающий за поведение и состояние персонажа.
 */
class APlayer
{
public:
    /**
     * @brief Конструктор по умолчанию для класса APlayer.
     */
    APlayer();

    /**
     * @brief Деструктор класса APlayer.
     */
    ~APlayer();

    /**
     * @brief Инициализация персонажа.
     *
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitPlayer(ASpriteManager& SpriteManager);

    /**
     * @brief Обработка выстрелов персонажа.
     *
     * @param BulletsVectorPtr Указатель на вектор пуль для добавления новой пули.
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void HandlePlayerShoots(std::vector<ABullet*>& BulletsVectorPtr, ASpriteManager& SpriteManager) const;

    /**
     * @brief Обработка движения персонажа.
     *
     * @param DeltaTime Время, прошедшее между последними кадрами.
     */
    void HandlePlayerMove(float DeltaTime);

    /**
     * @brief Обновление движения персонажа.
     *
     * @param DeltaTime Время, прошедшее между последними кадрами.
     * @param CollisionManager Менеджер коллизий для обработки столкновений с игровыми объектами.
     */
    void UpdatePlayerMove(float DeltaTime, const ACollisionManager& CollisionManager);
    
private:
    /**
     * @brief Обновление отображения шкалы здоровья персонажа.
     */
    void UpdatePlayerHealthBar();

public:
    /**
     * @briefУстановка скорости персонажа.
     *
     * @param NewVelocity Новая скорость персонажа.
     */
    void SetPlayerVelocity(const sf::Vector2f& NewVelocity);

    /**
     * @brief Установка прямоугольника персонажа.
     *
     * @param NewRect Новый прямоугольник, ограничивающий персонажа.
     */
    void SetPlayerRect(const sf::FloatRect& NewRect);

    /**
     * @brief Установка максимального здоровья персонажа.
     *
     * @param NewPlayerHealth Новое максимальное здоровье персонажа.
     */
    void SetPlayerMaxHealth(int NewPlayerHealth);
    
    /**
     * @brief Получение максимального здоровья персонажа.
     *
     * @return  Максимальное здоровье персонажа.
     */
    float GetPlayerMaxHealth() const;
    
    /**
     * @brief Получение текущей позиции персонажа.
     *
     * @return  Текущая позиция персонажа.
     */
    sf::Vector2f GetPlayerPosition() const;

    /**
     * @brief Получение прямоугольника, ограничивающего персонажа.
     *
     * @return  Прямоугольник, ограничивающий персонажа.
     */
    sf::FloatRect GetPlayerRect() const;

    /**
     * @brief Отрисовка персонажа на экране.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawPlayer(sf::RenderWindow& Window);

private:
    bool bCanJump;                       // Флаг, показывающий, может ли игрок прыгать
    bool bIsMoveRight;                   // Флаг, указывающий направление движения персонажа
    bool bIsOnLadder;                    // Флаг, указывающий, находится ли игрок на лестнице
    float PlayerHealth;                  // Текущее здоровье персонажа
    float PlayerSpeed;                   // Скорость перемещения персонажа
    float PlayerJumpSpeed;               // Скорость прыжка персонажа
    
    sf::Vector2f PlayerVelocity;         // Текущая скорость и направление движения персонажа
    sf::FloatRect PlayerRect;            // Прямоугольник, ограничивающий персонажа
    sf::Vector2f PlayerDrawPosition;     // Позиция для отрисовки персонажа
    
    sf::Texture* PlayerTexturePtr;       // Указатель на текстуру персонажа
    sf::Sprite PlayerSprite;             // Спрайт персонажа
    
    AHealthBar* HealthBarPtr;            // Указатель на шкалу здоровья персонажа
    sf::Texture* HealthBarTexturePtr;    // Текстура шкалы здоровья
    sf::Sprite HealthBarSprite;          // Спрайт шкалы здоровья

    // TODO: Для тестирования коллизий, удалить позже
    sf::RectangleShape PlayerRectCollision;  // Прямоугольная область для коллизий персонажа
};