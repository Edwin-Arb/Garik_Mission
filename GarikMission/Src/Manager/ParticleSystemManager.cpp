#include "ParticleSystemManager.h"

void AParticleSystemManager::InitParticleSystem()
{
    const std::string BulletTexturePath = ASSETS_PATH + "MainTiles/Bullets.png";
    
    AnimBulletDestroy.AnimTexture.loadFromFile(BulletTexturePath);
    AnimBulletDestroy.FrameSpeed = 5.f;
    AnimBulletDestroy.FrameRect.emplace_back(22, 70, static_cast<int>(BULLET_SIZE.x) + 5, static_cast<int>(BULLET_SIZE.y) + 5);
    AnimBulletDestroy.FrameRect.emplace_back(37, 71, static_cast<int>(BULLET_SIZE.x) + 5, static_cast<int>(BULLET_SIZE.y) + 5);
}

AAnimationManager AParticleSystemManager::GetAnimParticleSystem() const
{
    return AnimBulletDestroy;
}


