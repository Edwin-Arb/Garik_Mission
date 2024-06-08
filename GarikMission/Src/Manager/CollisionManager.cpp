// ReSharper disable CppMemberFunctionMayBeStatic
#include "CollisionManager.h"
#include "iostream"


ACollisionManager::ACollisionManager(APlayer& Player, AGameMap& GameMap)
    : PlayerRef(Player)
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
    // if (Bullet.GetBulletCollider().left <= 0.f || Bullet.GetBulletCollider().width >= SCREEN_WIDTH)
    // {
    //     return true;
    // }

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

void ACollisionManager::CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr,
                                                 std::vector<AEnemy*>& EnemysVectorPtr) const
{
    // Нужен, чтобы положить в него пули, которые столкнулись с препятствием.
    // Позже пройтись по нему и удалить из основного Вектора Пуль
    std::vector<ABullet*> BulletsToRemove;
    std::vector<AEnemy*> EnemysToRemove;

    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // Проверка пули с объектами карты и её границами       
        if (CheckBulletCollisionWithGameMap(*Bullet))
        {
            BulletsToRemove.emplace_back(Bullet);
        }

        for (AEnemy* Enemy : EnemysVectorPtr)
        {
            // Проверка пули с врагом
            if (CheckBulletCollisionWithEnemy(*Bullet, Enemy->GetEnemyRect()))
            {
                BulletsToRemove.emplace_back(Bullet);
                Enemy->SetEnemyHealth(Bullet->GetBulletDamage());

                // Вносим врага на удаление, если здоровье меньше 0
                if (Enemy->GetEnemyHealth() <= DEATH)
                {
                    EnemysToRemove.emplace_back(Enemy);
                }
            }
        }
    }

    // Очищаем вектор пуль, которые столкнулись с препятствием
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        // TODO вывод информации для тестов, позже удалить
        std::cout << "Removing bullet at position: " << BulletRemove->GetBulletCollider().left << ", "
            << BulletRemove->GetBulletCollider().top << std::endl;

        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove),
                               BulletsVectorPtr.end());
        delete BulletRemove;
    }

    // Очищаем вектор врагов, которых убили
    for (AEnemy* EnemyRemove : EnemysToRemove)
    {
        EnemysVectorPtr.erase(std::remove(EnemysVectorPtr.begin(), EnemysVectorPtr.end(), EnemyRemove),
                              EnemysVectorPtr.end());
        delete EnemyRemove;
    }
}

void ACollisionManager::HandlePlayerCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity,
                                                       bool& bCanJump) const
{
    // Проходим по всем объектам карты для проверки коллизии
    for (const auto& Obstacle : GameMapRef.GetCollisionVector())
    {
        // Проверка пересечения объекта с препятствием
        if (PawnRect.intersects(Obstacle))
        {
            // Вычисляем перекрытия с каждой стороны
            float OverlapLeft = (PawnRect.left + PawnRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - PawnRect.left;
            float OverlapTop = (PawnRect.top + PawnRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - PawnRect.top;

            // Определяем направление столкновения (слева или справа, сверху или снизу)
            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            // Находим минимальное перекрытие по осям X и Y
            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

            // Определяем направление столкновения и корректируем позицию объекта
            if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
            {
                // Горизонтальное столкновение
                PawnRect.left += FromLeft ? -OverlapLeft : OverlapRight;

                // Останавливаем горизонтальное движение
                ObjectVelocity.x = 0.f;
            }
            else
            {
                // Вертикальное столкновение
                PawnRect.top += FromTop ? -OverlapTop : OverlapBottom;

                // Останавливаем вертикальное движение
                ObjectVelocity.y = 0.f;

                // Если мы на земле, то даём возможность прыгать персонажу
                bCanJump = true;
            }
        }
    }
}
