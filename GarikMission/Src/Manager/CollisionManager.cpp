// ReSharper disable All
#include "CollisionManager.h"


ACollisionManager::ACollisionManager(AEnemy& Enemy, APlayer& Player, AGameMap& GameMap)
    : EnemeRef(Enemy)
      , PlayerRef(Player)
      , GameMapRef(GameMap)
{
}

// bool ACollisionManager::CheckPositionBulletWithScreen(const ABullet& Bullet) const
// {
//     // Проверяем столкновение пули с границами экрана
//     if (Bullet.GetBulletCollider().left <= 100.f ||
//         Bullet.GetBulletCollider().left + Bullet.GetBulletCollider().width >= SCREEN_WIDTH - 100.f)
//     {
//         return true;
//     }
//
//     // Проверяем столкновение пули с препятствиями на карте
//     for (const auto& Obstacle : GameMapRef.GetCollisionVector())
//     {
//         if (Bullet.GetBulletCollider().intersects(Obstacle))
//         {
//             return true;
//         }
//     }
//
//     return false;
// }

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

void ACollisionManager::CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr) const
{
    // Вектор для хранения пуль, которые нужно удалить
    std::vector<ABullet*> BulletsToRemove;

    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // TODO доделать коллизии пули с Объетами карты
        
        for (const auto& Obstacle : GameMapRef.GetCollisionVector())
        {
            if (Bullet->GetBulletCollider().intersects(Obstacle))
            {
                BulletsToRemove.emplace_back(Bullet);
                break;
            }
        }

        if (CheckBulletCollisionWithEnemy(*Bullet, EnemeRef.GetEnemyRect()))
        {
            BulletsToRemove.emplace_back(Bullet);
        }
    }

    // Удаляем пули, которые встретились с препятствием или врагом
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove),
                               BulletsVectorPtr.end());
        delete BulletRemove;
    }
}
