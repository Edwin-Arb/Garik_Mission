#include "CollisionManager.h"


ACollisionManager::ACollisionManager(AEnemy& Enemy)
    : EnemeRef(Enemy)
{
}

bool ACollisionManager::CheckPositionBulletWithScreen(const ABullet& Bullet) const
{
    // Если дистанция пули выходит за границы экрана, то возвращаем true
    return (Bullet.GetBulletCollider().left <= 100.f) ||
        (Bullet.GetBulletCollider().width >= SCREEN_WIDTH - 100.f);
}

bool ACollisionManager::CheckBulletCollisionWithEnemy(const ABullet& Bullet, const sf::FloatRect& EnemyRect) const
{
    if (Bullet.GetBulletCollider().width < EnemyRect.left ||
        Bullet.GetBulletCollider().left > EnemyRect.width)
    {
        return false;
    }
    if (Bullet.GetBulletCollider().height < EnemyRect.top ||
        Bullet.GetBulletCollider().top > EnemyRect.height)
    {
        return false;
    }
    return true;
}

void ACollisionManager::CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr)
{
    // Вектор для хранения индексов пуль, которые нужно удалить
    std::vector<ABullet*> BulletsToRemove;

    for (ABullet* Bullet : BulletsVectorPtr)
    {
        if (CheckPositionBulletWithScreen(*Bullet))
        {
            BulletsToRemove.emplace_back(Bullet);
        }

        if (CheckBulletCollisionWithEnemy(*Bullet, EnemeRef.GetEnemyRect()))
        {
            BulletsToRemove.emplace_back(Bullet);
        }
    }
    // ABullet* BulletRemove : BulletsToRemove
    // Удаляем пули, которые встрелись с препятствием
    for (ABullet* &BulletRemove : BulletsToRemove)         
    {
        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove),
                               BulletsVectorPtr.end());
        delete BulletRemove;
    }
}
