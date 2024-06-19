#pragma once
#include "../Abstract/APawn.h"
#include "../Bullet/Bullet.h"
#include "../Manager/AnimationManager.h"


// Предварительное объявление (Forward declaration of ACollisionManager)
class ACollisionManager;

/**
 * @brief Класс персонажа, отвечающий за поведение и состояние персонажа.
 */
class APlayer : public APawn
{
public:
    /**
     * @brief Конструктор для класса APlayer.
     */
    APlayer();

    /**
     * @brief Деструктор класса APlayer.
     */
    ~APlayer() override = default;

    /**
     * @brief Инициализация персонажа.
     *
     * @param SpriteManager Менеджер спрайтов для загрузки текстур.
     */
    void InitPlayer(ASpriteManager& SpriteManager);

    /**
     * @brief Получить по ссылки данные, если персонаж находить рядом с лестницей и может карабкаться по ней.
     * 
     * @return Текущие состояние заимодействие с лестницей.
     */
    bool &GetIsOnLadder();
 
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
     */
    void UpdatePlayerMove(float DeltaTime);

    /**
     * @brief Отрисовка персонажа на экране.
     *
     * @param Window Окно, в котором происходит отрисовка.
     */
    void DrawActor(sf::RenderWindow& Window) override;

private:
    bool bIsMoveRight;                   // Флаг, указывающий направление движения персонажа
    bool bIsOnLadder;                    // Флаг, указывающий, находится ли игрок на лестнице
    float PlayerJumpSpeed;               // Скорость прыжка персонажа

    AAnimationManager IdleAnimation;
    AAnimationManager WalkAnimation;
    AAnimationManager JumpUpAnimation;
    AAnimationManager JumpDownAnimation;

    AAnimationManager *CurrentAnimation;
};