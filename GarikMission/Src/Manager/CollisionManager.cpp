// ReSharper disable All
#include "CollisionManager.h"
#include "iostream"


ACollisionManager::ACollisionManager(AEnemy& Enemy, APlayer& Player, AGameMap& GameMap)
    : EnemeRef(Enemy)
      , PlayerRef(Player)
      , GameMapRef(GameMap)
{
}

bool ACollisionManager::CheckBulletCollisionWithGameMap(const ABullet& Bullet) const
{
    // Вычисляем положение пули с объектами коллизии карты
    for (const auto& Collision : GameMapRef.GetCollisionVector())
    {
        if (Bullet.GetBulletCollider().left < Collision.left + Collision.width &&
            Bullet.GetBulletCollider().left > Collision.left &&
            Bullet.GetBulletCollider().top < Collision.top + Collision.height &&
            Bullet.GetBulletCollider().top > Collision.top)
        {
            return true;
        }
    }

    // Вычисляем положение пули с границами карты
    if (Bullet.GetBulletCollider().left <= 0.f || Bullet.GetBulletCollider().width >= SCREEN_WIDTH)
    {
        return true;
    }
    
    return false;
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

void ACollisionManager::CheckBulletCollision(std::vector<ABullet*>& BulletsVectorPtr) const
{
    // Нужен, чтобы положить в него пули, которые столкнулись с препятствием.
    // Позже пройтись по нему и удалить из основного Вектора Пуль
    std::vector<ABullet*> BulletsToRemove;

    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // Проверка пули с объектами карты и её границами
        if (CheckBulletCollisionWithGameMap(*Bullet))
        {
            BulletsToRemove.emplace_back(Bullet);
        }

        // Проверка пули с врагом
        if (CheckBulletCollisionWithEnemy(*Bullet, EnemeRef.GetEnemyRect()))
        {
            BulletsToRemove.emplace_back(Bullet);
        }
    }

    // Очищаем вектор пуль, которые столкнулись с препятствием
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        // TODO вывод информации для тестов
        std::cout << "Removing bullet at position: " << BulletRemove->GetBulletCollider().left << ", " << BulletRemove->
            GetBulletCollider().top << std::endl;

        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove),
                               BulletsVectorPtr.end());
        delete BulletRemove;
    }
}
