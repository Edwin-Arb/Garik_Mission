#pragma once
#include "../Abstract/AActor.h"
#include "../Manager/CollisionManager.h"

class AKeyObject : public AActor
{
public:
    AKeyObject(const sf::Vector2f& StartPosition);

    bool GetKeyFound() const;
    void InitKeys(ASpriteManager& SpriteManager);
    void UpdateMoveKey(float DeltaTime);
    void SetKeyFound(bool WasKeyFound);
    int GetKeyCount() const;
    
    void DrawActor(sf::RenderWindow& Window) override;

private:
    bool JumpKey;
    bool bIsKeyFound;

public:
    static int KeysCount;
    static int KeysCountFound;
    //sf::Texture RedKeyTexture;
    // sf::Texture YellowKeyTexture;
    // sf::Texture GreenKeyTexture;
};
