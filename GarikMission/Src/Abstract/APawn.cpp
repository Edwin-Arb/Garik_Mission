#include "APawn.h"
#include <cassert>

/**
 * @brief Конструктор класса APawn.
 * Инициализирует флаги и параметры здоровья, скорости, а также создает объекты для шкалы здоровья и её текстуры.
 */
APawn::APawn()
    : bIsPawnJump(false)
      , PawnMaxHealth(0)
      , PawnCurrentHealth(0)
      , PawnSpeed(0)
      , PawnHealthBarPtr(new AHealthBar)
      , PawnHealthBarTexturePtr(new sf::Texture)
{
}

/**
 * @brief Деструктор класса APawn.
 * Удаляет объекты для шкалы здоровья и её текстуры.
 */
APawn::~APawn()
{
    delete PawnHealthBarPtr;
    delete PawnHealthBarTexturePtr;
}

/**
 * @brief Инициализирует шкалу здоровья пешки с текстурой.
 * 
 * @param TexturePath Путь к текстуре шкалы здоровья.
 * @param Size Размер шкалы здоровья.
 * @param Scale Масштаб шкалы здоровья.
 * @param FillColor Цвет заполнения шкалы здоровья.
 * @param BackgroundColor Цвет фона шкалы здоровья.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void APawn::InitPawnHealthBar(const std::string& TexturePath, const sf::Vector2f& Size, const sf::Vector2f& Scale,
                              const sf::Color& FillColor, const sf::Color& BackgroundColor,
                              ASpriteManager& SpriteManager)
{
    // Загрузка текстуры шкалы здоровья
    assert(PawnHealthBarTexturePtr->loadFromFile(TexturePath));

    // Инициализация шкалы здоровья
    PawnHealthBarPtr->InitHealthBar(Size, FillColor, BackgroundColor, SpriteManager);
    HealthBarSprite.setTexture(*PawnHealthBarTexturePtr);
    HealthBarSprite.setScale(Scale);
}

/**
 * @brief Инициализирует шкалу здоровья пешки без текстуры.
 * 
 * @param Size Размер шкалы здоровья.
 * @param FillColor Цвет заполнения шкалы здоровья.
 * @param BackgroundColor Цвет фона шкалы здоровья.
 * @param SpriteManager Менеджер спрайтов для обработки спрайтов объекта.
 */
void APawn::InitPawnHealthBar(const sf::Vector2f& Size,
                              const sf::Color& FillColor, const sf::Color& BackgroundColor,
                              ASpriteManager& SpriteManager)
{
    // Инициализация шкалы здоровья
    PawnHealthBarPtr->InitHealthBar(Size, FillColor, BackgroundColor, SpriteManager);
    HealthBarSprite.setTexture(*PawnHealthBarTexturePtr);
}

/**
 * @brief Получить по ссылке данные о состоянии прыжка пешки.
 * 
 * @return Текущее состояние прыжка пешки.
 */
bool& APawn::GetPawnCanJump()
{
    return bIsPawnJump;
}

/**
 * @brief Обновляет шкалу здоровья пешки с указанием позиции для отрисовки.
 * 
 * @param CurrentHealth Текущее здоровье.
 * @param MaxHealth Максимальное здоровье.
 * @param HealthBarPosition Позиция шкалы здоровья.
 * @param DrawHealthBarPosition Позиция для отрисовки шкалы здоровья.
 */
void APawn::UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                                const sf::Vector2f& HealthBarPosition,
                                const sf::Vector2f& DrawHealthBarPosition)
{
    // Обновление значений шкалы здоровья
    PawnHealthBarPtr->Update(CurrentHealth, MaxHealth);
    PawnHealthBarPtr->SetPosition(HealthBarPosition);
    HealthBarSprite.setPosition(DrawHealthBarPosition);

    // TODO: Временное восстановление здоровья для тестирования
    if (CurrentHealth <= DEATH)
    {
        PawnCurrentHealth = 400;
    }
}

/**
 * @brief Обновляет шкалу здоровья пешки без изменения позиции для отрисовки.
 * 
 * @param CurrentHealth Текущее здоровье.
 * @param MaxHealth Максимальное здоровье.
 * @param HealthBarPosition Позиция шкалы здоровья.
 */
void APawn::UpdatePawnHealthBar(float CurrentHealth, const float MaxHealth,
                                const sf::Vector2f& HealthBarPosition)
{
    // Обновление значений шкалы здоровья
    PawnHealthBarPtr->Update(CurrentHealth, MaxHealth);
    PawnHealthBarPtr->SetPosition(HealthBarPosition);

    // TODO: Временное восстановление здоровья для тестирования
    if (CurrentHealth <= DEATH)
    {
        PawnCurrentHealth = 400;
    }
}

/**
 * @brief Установить текущее здоровье пешки. Используется для изменения шкалы здоровья.
 * 
 * @param NewPawnCurrentHealth Новое текущее здоровье пешки.
 */
void APawn::SetPawnCurrentHealth(int NewPawnCurrentHealth)
{
    PawnCurrentHealth -= NewPawnCurrentHealth;
}

/**
 * @brief Получение текущего здоровья пешки.
 * 
 * @return Текущее здоровье пешки.
 */
float APawn::GetPawnCurrentHealth() const
{
    return PawnCurrentHealth;
}

/**
 * @brief Получение максимального здоровья пешки.
 * 
 * @return Максимальное здоровье пешки.
 */
float APawn::GetPawnMaxHealth() const
{
    return PawnMaxHealth;
}
