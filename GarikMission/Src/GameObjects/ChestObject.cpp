#include "ChestObject.h"

int AChestObject::ChestCount = 1;

AChestObject::AChestObject(const sf::Vector2f& StartPositionChest)
    : bIsChestOpen(false),
      CurrentChestAnimation(nullptr)
{
    ActorSize = {16.f, 13.f};

    ActorCollisionRect = {StartPositionChest.x, StartPositionChest.y, ActorSize.x, ActorSize.y};
    // x = 120.f, y = 630.f
}

void AChestObject::InitChest(ASpriteManager& SpriteManager)
{
    const std::string ChestTexturePath = ASSETS_PATH + "MainTiles/Chests.png";
    const sf::Vector2f ChestOrigin = {0.5f, 0.5f};

    switch (ChestCount)
    {
    case 1:
        {
            // Красный сундук
            const sf::IntRect RedChestRectTexture = sf::IntRect(0, 1, ActorSize.x, ActorSize.y);
            AActor::InitActorTexture(ChestTexturePath, RedChestRectTexture, ActorSize, ChestOrigin, SpriteManager);

            // Анимация открытия сундука
            RedChestOpenAnimation.AnimTexture.loadFromFile(ChestTexturePath);
            RedChestOpenAnimation.FrameSpeed = 5.f;
            RedChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(RedChestRectTexture));
            RedChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(16, 1, ActorSize.x, ActorSize.y));
            RedChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(48, 1, ActorSize.x, ActorSize.y));
            RedChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(64, 1, ActorSize.x, ActorSize.y));

            RedChestOpenAnimation.SetStopAtLastFrame(true); // Остановить на последнем кадре

            CurrentChestAnimation = &RedChestOpenAnimation; // Назначаем текущую анимацию

            break;
        }
    case 2:
        {
            // Зеленый сундук
            const sf::IntRect GreenChestRectTexture = sf::IntRect(0, 16, 16, 13);
            AActor::InitActorTexture(ChestTexturePath, GreenChestRectTexture, ActorSize, ChestOrigin, SpriteManager);

            // Анимация открытия сундука
            GreenChestOpenAnimation.AnimTexture.loadFromFile(ChestTexturePath);
            GreenChestOpenAnimation.FrameSpeed = 5.f;
            GreenChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(GreenChestRectTexture));
            GreenChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(16, 16, ActorSize.x, ActorSize.y));
            GreenChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(48, 16, ActorSize.x, ActorSize.y));
            GreenChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(64, 16, ActorSize.x, ActorSize.y));

            GreenChestOpenAnimation.SetStopAtLastFrame(true); // Остановить на последнем кадре

            CurrentChestAnimation = &GreenChestOpenAnimation; // Назначаем текущую анимацию

            break;
        }
    case 3:
        {
            // Желтый сундук
            const sf::IntRect YellowChestRectTexture = sf::IntRect(0, 31, 16, 13);
            AActor::InitActorTexture(ChestTexturePath, YellowChestRectTexture, ActorSize, ChestOrigin, SpriteManager);

            // Анимация открытия сундука
            YellowChestOpenAnimation.AnimTexture.loadFromFile(ChestTexturePath);
            YellowChestOpenAnimation.FrameSpeed = 3.f;
            YellowChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(YellowChestRectTexture));
            YellowChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(16, 31, ActorSize.x, ActorSize.y));
            YellowChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(48, 31, ActorSize.x, ActorSize.y));
            YellowChestOpenAnimation.FrameRect.emplace_back(sf::IntRect(64, 31, ActorSize.x, ActorSize.y));

            YellowChestOpenAnimation.SetStopAtLastFrame(true); // Остановить на последнем кадре

            CurrentChestAnimation = &YellowChestOpenAnimation; // Назначаем текущую анимацию

            break;
        }
    default:
        {
            // Если не совпадает ни один сундук
            break;
        }
    }

    ActorSprite.setScale(1.f, 1.f);
}

void AChestObject::ChestUpdateAnimation(float DeltaTime)
{
    if (bIsChestOpen && CurrentChestAnimation)
    {
        if (!CurrentChestAnimation->IsAnimationFinished())
        {
            ActorSprite.setTextureRect(CurrentChestAnimation->GetCurrentFrame());
            CurrentChestAnimation->AnimationUpdate(DeltaTime);
        }
    }

    // Обновление позиции спрайта персонажа и его коллизии
    ActorDrawPosition = {
        ActorCollisionRect.left + ActorSize.x / 2.f,
        ActorCollisionRect.top + ActorSize.y / 2.f
    };

    ActorSprite.setPosition(ActorDrawPosition);
}

void AChestObject::OpenChest()
{
    bIsChestOpen = true;
}


void AChestObject::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}
