#pragma once
#include "../Constants.h"
#include "../Abstract/AActor.h"
#include "../Manager/AnimationManager.h"

class AChestObject : public AActor
{
public:
    AChestObject() = default;
    AChestObject(const sf::Vector2f &StartPositionChest);
    ~AChestObject() override = default;
    
    void InitChest(ASpriteManager& SpriteManager);
    void ChestUpdateAnimation(float DeltaTime);
    void OpenChest(); // функция для открытия сундука
    void DrawActor(sf::RenderWindow& Window) override;

private:
    bool bIsChestOpen;
    
public:
    static int ChestCount;
    
private:
    AAnimationManager RedChestOpenAnimation;
    AAnimationManager GreenChestOpenAnimation;
    AAnimationManager YellowChestOpenAnimation;

    AAnimationManager *CurrentChestAnimation; // Указатель на текущию анимацию сундука
};
