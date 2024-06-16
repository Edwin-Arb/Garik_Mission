#pragma once
#include "../Abstract/AActor.h"
#include "../Bullet/Bullet.h"
#include "../UserInterface/HealthBar.h"

// Предварительное объявление (Forward declaration of ACollisionManager)
class ACollisionManager;

/**
 * @brief Класс персонажа, отвечающий за поведение и состояние персонажа.
 */
class APlayer : public AActor
{
public:
    /**
     * @brief Конструктор по умолчанию для класса APlayer.
     */
    APlayer();

    /**
     * @brief Деструктор класса APlayer.
     */
    ~APlayer() override;

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
     * @brief Отрисовка персонажа на экране.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawActor(sf::RenderWindow& Window) override;

private:
    bool bCanJump;                       // Флаг, показывающий, может ли игрок прыгать
    bool bIsMoveRight;                   // Флаг, указывающий направление движения персонажа
    bool bIsOnLadder;                    // Флаг, указывающий, находится ли игрок на лестнице
    float PlayerHealth;                  // Текущее здоровье персонажа
    float PlayerSpeed;                   // Скорость перемещения персонажа
    float PlayerJumpSpeed;               // Скорость прыжка персонажа
       
    AHealthBar* HealthBarPtr;            // Указатель на шкалу здоровья персонажа
    sf::Texture* HealthBarTexturePtr;    // Текстура шкалы здоровья
    sf::Sprite HealthBarSprite;          // Спрайт шкалы здоровья

    // TODO: Для тестирования коллизий, удалить позже
    sf::RectangleShape PlayerRectCollision;  // Прямоугольная область для коллизий персонажа
};