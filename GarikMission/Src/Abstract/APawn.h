#pragma once
#include "AActor.h"
#include "../UserInterface/HealthBar.h"

/**
 * @brief Класс APawn, производный от AActor. Представляет собой управляемую пешку с функционалом здоровья.
 */
class APawn : public AActor
{
public:
    /**
     * @brief Конструктор класса APawn.
     */
    APawn();

    /**
     * @brief Деструктор класса APawn.
     */
    ~APawn() override;

    /**
     * @brief Инициализирует шкалу здоровья пешки.
     * 
     * @param TexturePath Путь к текстуре шкалы здоровья.
     * @param Size Размер шкалы здоровья.
     * @param Scale Масштаб шкалы здоровья.
     * @param FillColor Цвет заполнения шкалы здоровья.
     * @param BackgroundColor Цвет фона шкалы здоровья.
     * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
     */
    void InitPawnHealthBar(const std::string& TexturePath, const sf::Vector2f& Size, const sf::Vector2f& Scale,
                           const sf::Color& FillColor, const sf::Color& BackgroundColor,
                           ASpriteManager& SpriteManager);

    /**
     * @brief Инициализирует шкалу здоровья пешки без текстуры.
     * 
     * @param Size Размер шкалы здоровья.
     * @param FillColor Цвет заполнения шкалы здоровья.
     * @param BackgroundColor Цвет фона шкалы здоровья.
     * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
     */
    void InitPawnHealthBar(const sf::Vector2f& Size,
                           const sf::Color& FillColor, const sf::Color& BackgroundColor,
                           ASpriteManager& SpriteManager);

    /**
     * @brief Получить по ссылке данные о состоянии прыжка пешки.
     * 
     * @return Текущее состояние прыжка пешки.
     */
    bool& GetPawnCanJump();

    /**
     * @brief Обновляет шкалу здоровья пешки.
     * 
     * @param CurrentHealth Текущее здоровье.
     * @param MaxHealth Максимальное здоровье.
     * @param HealthBarPosition Позиция шкалы здоровья.
     * @param DrawHealthBarPosition Позиция для отрисовки шкалы здоровья.
     */
    void UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                             const sf::Vector2f& HealthBarPosition,
                             const sf::Vector2f& DrawHealthBarPosition);

    /**
     * @brief Обновляет шкалу здоровья пешки без изменения позиции для отрисовки.
     * 
     * @param CurrentHealth Текущее здоровье.
     * @param MaxHealth Максимальное здоровье.
     * @param HealthBarPosition Позиция шкалы здоровья.
     */
    void UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                             const sf::Vector2f& HealthBarPosition);

    /**
     * @brief Установить текущее здоровье пешки. Используется для изменения шкалы здоровья.
     * 
     * @param NewPawnCurrentHealth Новое текущее здоровье пешки.
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
    bool bIsPawnJump; // Флаг, показывающий, может ли пешка прыгать
    float PawnMaxHealth; // Максимальное здоровье пешки.
    float PawnCurrentHealth; // Текущее здоровье пешки.
    float PawnSpeed; // Скорость перемещения пешки

    AHealthBar* PawnHealthBarPtr; // Указатель на шкалу здоровья персонажа
    sf::Texture* PawnHealthBarTexturePtr; // Указатель на текстуру шкалы здоровья
    sf::Sprite HealthBarSprite; // Спрайт шкалы здоровья
};
