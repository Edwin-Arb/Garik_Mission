#pragma once
#include "../Constants.h"
#include "AnimationManager.h"

class AParticleSystemManager
{
public:
    void InitParticleSystem();
    AAnimationManager GetAnimParticleSystem() const;

private:
    AAnimationManager AnimBulletDestroy;
};
