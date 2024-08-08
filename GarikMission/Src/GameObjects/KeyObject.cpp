#include "KeyObject.h"

int AKeyObject::KeysCount = 0;
int AKeyObject::KeysCountFound = 0;

AKeyObject::AKeyObject(const sf::Vector2f& StartPosition)
    : bIsKeyFound(false),
      JumpKey(true)
{
    ++KeysCount; // Увеличиваем счетчик ключей
    ActorVelocity = {0.f, 0.f};
    ActorSize = {14, 9};
    ActorCollisionRect = {StartPosition.x, StartPosition.y, ActorSize.x, ActorSize.y};
}

bool AKeyObject::GetKeyFound() const
{
    return bIsKeyFound;
}

void AKeyObject::InitKeys(ASpriteManager& SpriteManager)
{
    // Инициализация 3 видов ключей
    const std::string KeysTexturePath = ASSETS_PATH + "MainTiles/AllObjects.png";
    const sf::IntRect RedKeyRectTexture = sf::IntRect(1, 20, ActorSize.x, ActorSize.y);
    const sf::IntRect YellowKeyRectTexture = sf::IntRect(17, 20, ActorSize.x, ActorSize.y);
    const sf::IntRect GreenKeyRectTexture = sf::IntRect(33, 20, ActorSize.x, ActorSize.y);
    const sf::Vector2f KeyOrigin = {0.5f, 0.5f};

    switch (KeysCount)
    {
    case 1:
        {
            // Красный ключ
            AActor::InitActorTexture(KeysTexturePath, RedKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);
            break;
        }
    case 2:
        {
            // Желтый ключ
            AActor::InitActorTexture(KeysTexturePath, YellowKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);
            break;
        }
    case 3:
        {
            // Зеленый ключ
            AActor::InitActorTexture(KeysTexturePath, GreenKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);
            break;
        }
    default:
        {
            // Красный ключ
            AActor::InitActorTexture(KeysTexturePath, RedKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);
            break;
        }
    }


    // Красный ключ
    //AActor::InitActorTexture(KeysTexturePath, GreenKeyRectTexture, ActorSize, KeyOrigin, SpriteManager);
    ActorSprite.setScale(0.8f, 0.8f);

    // Желтый ключ
    // AActor::InitActorTexture(KeysTexturePath, YellowKeyRectTexture, KeySize, KeyOrigin, SpriteManager);

    // Зеленый ключ
    // AActor::InitActorTexture(KeysTexturePath, GreenKeyRectTexture, KeySize, KeyOrigin, SpriteManager);
}

void AKeyObject::UpdateMoveKey(float DeltaTime)
{
    // TODO: Тестовая реализация появления(лёгкого прыжка вверх), после убийсва босса
    if (JumpKey)
    {
        float KeyVelocityJump = 120.f;

        ActorVelocity.y = KeyVelocityJump * DeltaTime;
        JumpKey = false;
    }

    // Применение гравитации для падения персонажа
    ActorVelocity.y += -2.f * DeltaTime;

    // Обновление позиции персонажа на основе скорости
    ActorCollisionRect.left += ActorVelocity.x;
    ActorCollisionRect.top -= ActorVelocity.y;

    // Обновление позиции спрайта персонажа и его коллизии
    ActorDrawPosition = {
        ActorCollisionRect.left + (ActorSize.x / 2.f),
        ActorCollisionRect.top + (ActorSize.y / 2.f)
    };

    ActorSprite.setPosition(ActorDrawPosition);
}

void AKeyObject::SetKeyFound(bool WasKeyFound)
{
    bIsKeyFound = WasKeyFound;
}

int AKeyObject::GetKeyCount() const
{
    return KeysCount;
}

void AKeyObject::DrawActor(sf::RenderWindow& Window)
{
    Window.draw(ActorSprite);
}
