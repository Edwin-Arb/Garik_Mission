#pragma once
#include "AActor.h"
#include "../UserInterface/HealthBar.h"

class APawn : public AActor
{
public:
    APawn();
    ~APawn() override;
    
    void InitPawnHealthBar(const std::string &TexturePath, const sf::Vector2f& Size, const sf::Vector2f &Scale,
                           const sf::Color& FillColor, const sf::Color& BackgroundColor,
                           ASpriteManager &SpriteManager);
 
    void InitPawnHealthBar(const sf::Vector2f& Size, const sf::Vector2f &Scale,
                           const sf::Color& FillColor, const sf::Color& BackgroundColor,
                           ASpriteManager &SpriteManager);

    
    /**
     * @brief Получить по ссылки данные о состояние прыжка пешки.
     * 
     * @return Текущие состояние прыжка пешки.
     */
    bool &GetPawnCanJump();
    
    void UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                             const sf::Vector2f& HealthBarPosition,
                             const sf::Vector2f& DrawHealthBarPosition);

    void UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                             const sf::Vector2f& HealthBarPosition);

    /**
     * @brief Установить текущие здоровье пешки.
     * Используеться, для изменения шкалы здоровья.
     *
     * @param NewPawnCurrentHealth Новое текущие здоровье пешки.
     */
    void SetPawnCurrentHealth(int NewPawnCurrentHealth);

    /**
     * @brief Получение текущего здоровья пешки.
     *
     * @return Текущее здоровье пешки.
     */
    float GetPawnCurrentHealth() const;
    
    /**
     * @brief Получение максимального здоровья пешки.
     *
     * @return Максимальное здоровье пешки.
     */
    float GetPawnMaxHealth() const;

protected:
    bool bIsPawnJump;                        // Флаг, показывающий, может ли пешка прыгать
    float PawnMaxHealth;                     // Максимальное здоровье пешки.
    float PawnCurrentHealth;                 // Текущие здоровье пешки.
    float PawnSpeed;                         // Скорость перемещения пешки.
    
    AHealthBar* PawnHealthBarPtr;            // Указатель на шкалу здоровья персонажа
    sf::Texture* PawnHealthBarTexturePtr;    // Указатель на текстуру шкалы здоровья
    sf::Sprite HealthBarSprite;              // Спрайт шкалы здоровья
};
