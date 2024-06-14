#include "CollisionManager.h"
#include "iostream"


/**
 * @brief Конструктор класса ACollisionManager.
 * @param Player Ссылка на объект игрока.
 * @param GameMap Ссылка на объект игровой карты.
 */
ACollisionManager::ACollisionManager(APlayer& Player, AGameMap& GameMap)
    : PlayerRef(Player)
      , GameMapRef(GameMap)
{
}

/**
 * @brief Проверяет столкновение пули с игровой картой.
 * @param Bullet Пуля для проверки столкновения.
 * @return true, если есть столкновение, иначе false.
 */
bool ACollisionManager::CheckBulletCollisionWithGameMap(const ABullet& Bullet) const
{
    // Перебираем все объекты коллизий на игровой карте
    for (const auto& Collision : GameMapRef.GetGameMapCollisionVector())
    {
        // Проверяем, пересекается ли коллайдер пули с текущим объектом коллизии
        if (Bullet.GetBulletCollider().left < Collision.left + Collision.width &&
            Bullet.GetBulletCollider().left > Collision.left &&
            Bullet.GetBulletCollider().top < Collision.top + Collision.height &&
            Bullet.GetBulletCollider().top > Collision.top)
        {
            return true; // Столкновение обнаружено
        }

        // TODO: Добавить Window окно, если пуля заходит за его границы, то очищаем её
        // // Проверяем, находится ли пуля за пределами игровой области
        // if (Bullet.GetBulletCollider().left < 0.f ||
        //     Bullet.GetBulletCollider().left + Bullet.GetBulletCollider().width > 1280.f)
        // {
        //     return true; // Обнаружено столкновение с границей
        // }
    }

    return false; // Столкновение не обнаружено
}

/**
 * @brief Проверяет столкновение пули с пешкой (персонажем).
 * @param Bullet Пуля для проверки столкновения.
 * @param PawnRect Прямоугольник коллизии пешки.
 * @return true, если есть столкновение, иначе false.
 */
bool ACollisionManager::CheckBulletCollisionWithPawn(const ABullet& Bullet, const sf::FloatRect& PawnRect) const
{
    // Проверяем, пересекается ли коллайдер пули с прямоугольником пешки
    return Bullet.GetBulletCollider().intersects(PawnRect);
}

/**
 * @brief Проверяет столкновения всех пуль с препятствиями и врагами.
 * @param BulletsVectorPtr Указатель на вектор пуль.
 * @param EnemysVectorPtr Указатель на вектор врагов.
 */
void ACollisionManager::CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr,
                                                 std::vector<AEnemy*>& EnemysVectorPtr) const
{
    // Векторы для хранения пуль и врагов для удаления из основных векторов
    std::vector<ABullet*> BulletsToRemove;
    std::vector<AEnemy*> PawnsToRemove;

    // Перебираем все пули
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // Проверяем столкновение с игровой картой и её границами       
        if (CheckBulletCollisionWithGameMap(*Bullet))
        {
            BulletsToRemove.emplace_back(Bullet); // Добавляем пулю для удаления
        }

        // Проверяем столкновение с игроком
        if (CheckBulletCollisionWithPawn(*Bullet, PlayerRef.GetPlayerRect()) &&
            Bullet->GetBulletType() == EBulletType::EBT_ShootAtPlayer)
        {
            BulletsToRemove.emplace_back(Bullet); // Добавляем пулю для удаления

            // TODO: Уменьшаем здоровье игрока, пока он не умрет
            if (PlayerRef.GetPlayerMaxHealth() > DEATH)
            {
                PlayerRef.SetPlayerMaxHealth(Bullet->GetBulletDamage());
            }
        }

        // Перебираем всех врагов
        for (AEnemy* Enemy : EnemysVectorPtr)
        {
            // Проверяем столкновение с врагом
            if (CheckBulletCollisionWithPawn(*Bullet, Enemy->GetEnemyRect()) &&
                Bullet->GetBulletType() == EBulletType::EBT_ShootAtEnemy)
            {
                BulletsToRemove.emplace_back(Bullet); // Добавляем пулю для удаления

                // Уменьшаем здоровье врага
                Enemy->SetEnemyHealth(Bullet->GetBulletDamage());

                // Добавляем врага для удаления, если его здоровье меньше нуля
                if (Enemy->GetEnemyHealth() <= DEATH)
                {
                    PawnsToRemove.emplace_back(Enemy);
                }
            }
        }
    }

    // Удаляем пули, столкнувшиеся с препятствиями или врагами
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        // TODO: Выводим информацию для тестирования, удалить позже
        std::cout << "Delete a bullet at position: " << BulletRemove->GetBulletCollider().left << ", "
            << BulletRemove->GetBulletCollider().top << std::endl;

        BulletsVectorPtr.erase(std::remove(BulletsVectorPtr.begin(), BulletsVectorPtr.end(), BulletRemove),
                               BulletsVectorPtr.end());
        delete BulletRemove;
    }

    // Удаляем врагов, которых убили пули
    for (AEnemy* PawnRemove : PawnsToRemove)
    {
        EnemysVectorPtr.erase(std::remove(EnemysVectorPtr.begin(), EnemysVectorPtr.end(), PawnRemove),
                              EnemysVectorPtr.end());
        delete PawnRemove;
    }
}

/**
 * @brief Обрабатывает столкновение игрока с игровой картой.
 * @param PawnRect Прямоугольник коллизии игрока.
 * @param ObjectVelocity Вектор скорости объекта игрока.
 * @param bCanJump Может ли игрок прыгать.
 * @param bCanClimb Может ли игрок карабкаться.
 */
void ACollisionManager::HandlePlayerCollisionWithGameMap(sf::FloatRect& PawnRect, sf::Vector2f& ObjectVelocity,
                                                         bool& bCanJump, bool& bCanClimb) const
{
    bCanClimb = false;

    // Проверяем столкнование игрока с препятствиями игровой карты
    for (const auto& Obstacle : GameMapRef.GetGameMapCollisionVector())
    {
        if (PawnRect.intersects(Obstacle))
        {
            // Рассчитываем перекрытие по всем сторонам препятствия
            float OverlapLeft = (PawnRect.left + PawnRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - PawnRect.left;
            float OverlapTop = (PawnRect.top + PawnRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - PawnRect.top;

            // Определяем направление, откуда пришло столкновение
            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            // Выбираем минимальное перекрытие по X и Y
            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

            // Если перекрытие по X меньше, чем по Y, это горизонтальное столкновение
            if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
            {
                // Корректируем положение игрока по X
                PawnRect.left += FromLeft ? -OverlapLeft : OverlapRight;
                // Останавливаем горизонтальное движение игрока
                ObjectVelocity.x = 0.f;
            }
            else // В противном случае это вертикальное столкновение
            {
                // Корректируем положение игрока по Y
                PawnRect.top += FromTop ? -OverlapTop : OverlapBottom;
                // Останавливаем вертикальное движение игрока
                ObjectVelocity.y = 0.f;

                // Если игрок коснулся земли сверху, он может прыгать
                if (FromTop)
                {
                    bCanJump = true;
                }
            }
        }
    }

    // Проверяем, находится ли игрок на лестнице
    for (const auto& Ladder : GameMapRef.GetLadderCollisionVector())
    {
        if (PawnRect.intersects(Ladder))
        {
            bCanClimb = true;
            break;
        }
    }
}
