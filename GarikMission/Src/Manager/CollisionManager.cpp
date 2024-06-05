// ReSharper disable CppMemberFunctionMayBeStatic
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

void ACollisionManager::CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr) const
{
    // Нужен, чтобы положить в него пули, которые столкнулись с препятствием.
    // Позже пройтись по нему и удалить из основного Вектора Пуль
    std::vector<ABullet*> BulletsToRemove;

    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // Проверка пули с объектами карты и её границами       // Проверка пули с врагом
        if (CheckBulletCollisionWithGameMap(*Bullet) || CheckBulletCollisionWithEnemy(*Bullet, EnemeRef.GetEnemyRect()))
        {
            BulletsToRemove.emplace_back(Bullet);
        }

        // Проверка пули с врагом
        // if (CheckBulletCollisionWithEnemy(*Bullet, EnemeRef.GetEnemyRect()))
        // {
        //     BulletsToRemove.emplace_back(Bullet);
        // }
    }

    // Очищаем вектор пуль, которые столкнулись с препятствием
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        // TODO вывод информации для тестов
        std::cout << "Removing bullet at position: " << BulletRemove->GetBulletCollider().left << ", "
            << BulletRemove->GetBulletCollider().top << std::endl;

        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove),
                               BulletsVectorPtr.end());
        delete BulletRemove;
    }
}

void ACollisionManager::HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity) const
{
    for (const auto& Obstacle : GameMapRef.GetCollisionVector())
    {
        if (PawnRect.intersects(Obstacle))
        {
            // Рассчитываем перекрытие по каждой стороне
            float OverlapLeft = (PawnRect.left + PawnRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - PawnRect.left;
            float OverlapTop = (PawnRect.top + PawnRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - PawnRect.top;

            // Определяем, с какой стороны произошло столкновение
            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            // Выбираем наименьшее перекрытие
            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

            // Применяем коррекцию к позиции объекта
            if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
            {
                // Горизонтальное столкновение
                //PawnRect.left += FromLeft ? -MinOverlapX : MinOverlapX;

                // Останавливаем горизонтальное движение
                ObjectVelocity.x = 0.f;
            }
            else
            {
                // Вертикальное столкновение
                //PawnRect.top += FromTop ? -MinOverlapY : MinOverlapY;

                // Останавливаем вертикальное движение
                ObjectVelocity.y = 0.f;
            }
        }
    }
}

void ACollisionManager::HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity,
                                                       bool& bCanJump) const
{
    for (const auto& Obstacle : GameMapRef.GetCollisionVector())
    {
        if (PawnRect.intersects(Obstacle))
        {
            float OverlapLeft = (PawnRect.left + PawnRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - PawnRect.left;
            float OverlapTop = (PawnRect.top + PawnRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - PawnRect.top;

            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

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

                // Если мы на земле, то даём прыгать персонажу или врагу
                bCanJump = true;
            }
        }
    }
}