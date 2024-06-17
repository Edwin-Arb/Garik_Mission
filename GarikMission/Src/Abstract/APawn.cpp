#include "APawn.h"
#include <cassert>


APawn::APawn()
    : PawnMaxHealth(0)
      , PawnCurrentHealth(0)
      , PawnSpeed(0)
      , PawnHealthBarPtr(new AHealthBar)
      , PawnHealthBarTexturePtr(new sf::Texture)
{
}

APawn::~APawn()
{
    delete PawnHealthBarPtr;
    delete PawnHealthBarTexturePtr;
}

void APawn::InitPawnHealthBar(const std::string& TexturePath, const sf::Vector2f& Size, const sf::Vector2f& Scale,
                              const sf::Color& FillColor, const sf::Color& BackgroundColor,
                              ASpriteManager& SpriteManager)
{
    // Загрузка текстур шкалы здоровья
    assert(PawnHealthBarTexturePtr->loadFromFile(TexturePath));

    // Инициализация шкалы здоровья
    PawnHealthBarPtr->InitHealthBar(Size, FillColor, BackgroundColor, SpriteManager);
    HealthBarSprite.setTexture(*PawnHealthBarTexturePtr);
    HealthBarSprite.setScale(Scale);
}

void APawn::InitPawnHealthBar(const sf::Vector2f& Size, const sf::Vector2f& Scale,
                              const sf::Color& FillColor, const sf::Color& BackgroundColor,
                              ASpriteManager& SpriteManager)
{
    // Инициализация шкалы здоровья
    PawnHealthBarPtr->InitHealthBar(Size, FillColor, BackgroundColor, SpriteManager);
    HealthBarSprite.setTexture(*PawnHealthBarTexturePtr);
    HealthBarSprite.setScale(Scale);
}

/**
 * @brief Обновление состояния шкалы здоровья персонажа.
 * Рассчитывает текущее состояние шкалы и её положение на экране.
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
* @brief Установить текущие здоровье пешки.
* Используеться, для изменения шкалы здоровья.
*
* @param NewPawnCurrentHealth Новое текущие здоровье пешки.
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
* @brief Получение текущего значения здоровья пешки.
* 
* @return Текущее значение здоровья пешки.
*/
float APawn::GetPawnMaxHealth() const
{
    return PawnMaxHealth;
}