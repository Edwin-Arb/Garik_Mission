#pragma once
#include "../Constants.h"

class AAnimationManager
{
public:
    AAnimationManager();
    ~AAnimationManager() = default;

    void AnimationUpdate(float DeltaTime);
    void SetCurrentFrame(const int Frame);

    int GetCurrentFrameIndex() const;

    sf::IntRect GetCurrentFrame() const;

    float FrameSpeed;
    std::vector<sf::IntRect> FrameRect;
    sf::Texture AnimTexture;
    
private:
    float CurrentFrameIndex;
    
};
