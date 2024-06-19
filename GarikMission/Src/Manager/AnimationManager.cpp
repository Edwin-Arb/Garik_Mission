#include "AnimationManager.h"

AAnimationManager::AAnimationManager()
    : FrameSpeed(0.1f)
      ,CurrentFrameIndex(0)

{}

void AAnimationManager::AnimationUpdate(float DeltaTime)
{
    CurrentFrameIndex += FrameSpeed * DeltaTime;

    if (CurrentFrameIndex >= FrameRect.size())
    {
        CurrentFrameIndex = 0;
    }
}

void AAnimationManager::SetCurrentFrame(const int Frame)
{
    CurrentFrameIndex = Frame;
}

int AAnimationManager::GetCurrentFrameIndex() const
{
    return static_cast<int>(CurrentFrameIndex);
}

sf::IntRect AAnimationManager::GetCurrentFrame() const
{
    return FrameRect[CurrentFrameIndex];
}
