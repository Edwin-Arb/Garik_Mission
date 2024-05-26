#include "CollisionManager.h"


bool ACollisionManager::CheckPositionBulletWithScreen(const ABullet &Bullet) const
{
    // Если дистанция пули выходит за границы экрана, то возвращаем true
    return (Bullet.GetBulletCollider().left <= 100.f) ||
           (Bullet.GetBulletCollider().width >= SCREEN_WIDTH - 100.f);
}

bool ACollisionManager::CheckBulletCollisionWithEnemy(const ABullet &Bullet, const sf::FloatRect& Enemy) const
{
    // Проверка столкновения с врагом
    //return BulletRect.left >= Enemy.left && BulletRect.top >= Enemy.top && BulletRect.width >= Enemy.width && BulletRect.height >= Enemy.height;
    return Bullet.GetBulletCollider().intersects(Enemy);
}

void ACollisionManager::CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr)
{
    // Вектор для хранения индексов(итераторов) пуль, которые нужно удалить
    std::vector<ABullet*> IndicesToRemove;
    
    for (ABullet *Bullet : BulletsVectorPtr)
    {
        if (CheckPositionBulletWithScreen(*Bullet))
        {
            IndicesToRemove.emplace_back(Bullet);
        }
    }

    // Удаляем пули, которые встрелись с препятствием, начиная с конца вектора
    for (ABullet *BulletRemove : IndicesToRemove)
    {
        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove), BulletsVectorPtr.end());
        delete BulletRemove;
    }
}