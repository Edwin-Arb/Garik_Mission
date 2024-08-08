// ReSharper disable CppMemberFunctionMayBeStatic
#include "CollisionManager.h"

/**
 * @brief Конструктор класса ACollisionManager.
 * 
 * @param Player Ссылка на объект персонажа.
 * @param GameMap Ссылка на объект игровой карты.
 */
ACollisionManager::ACollisionManager(APlayer& Player, AGameMap& GameMap)
    : PlayerRef(Player)
      , GameMapRef(GameMap)
{
    // Конструктор инициализирует ссылки на персонажа и игровую карту.
}

/**
* @brief Проверяет пересечение двух прямоугольников.
* 
* @param Rect1 Первый прямоугольник.
* @param Rect2 Второй прямоугольник.
* @return true, если прямоугольники пересекаются, иначе false.
*/
bool ACollisionManager::CheckCollision(const sf::FloatRect& Rect1, const sf::FloatRect& Rect2)
{
    return Rect1.intersects(Rect2);
}

/**
 * @brief Проверяет столкновение пули с игровой картой.
 * 
 * @param Bullet Пуля для проверки столкновения.
 * @return true, если есть столкновение, иначе false.
 */
bool ACollisionManager::CheckBulletCollisionWithGameMap(AActor& Bullet) const
{
    // Перебираем все объекты коллизий на игровой карте
    for (const auto& Collision : GameMapRef.GetGameMapCollisionVector())
    {
        // Проверяем, пересекается ли коллайдер пули с текущим объектом коллизии
        if (Bullet.GetActorCollisionRect().left < Collision.left + Collision.width &&
            Bullet.GetActorCollisionRect().left > Collision.left &&
            Bullet.GetActorCollisionRect().top < Collision.top + Collision.height &&
            Bullet.GetActorCollisionRect().top > Collision.top)
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
 * 
 * @param Bullet Пуля для проверки столкновения.
 * @param PawnRect Прямоугольник коллизии пешки.
 * @return true, если есть столкновение, иначе false.
 */
bool ACollisionManager::CheckBulletCollisionWithPawn(AActor& Bullet, const sf::FloatRect& PawnRect) const
{
    // Проверяем, пересекается ли коллайдер пули с прямоугольником пешки
    return Bullet.GetActorCollisionRect().intersects(PawnRect);
}

/**
 * @brief Проверяет столкновения всех пуль с препятствиями и врагами.
 * 
 * @param BulletsVectorPtr Указатель на вектор пуль.
 * @param EnemyVectorPtr Указатель на вектор врагов.
 * @param Player Ссылка на персонажа.
 * @param ParticleSystem Ссылка на менеджер частиц.  // TODO: Тест анимация частиц.
 */
void ACollisionManager::CheckAllBulletCollisions(std::vector<ABullet*>& BulletsVectorPtr,
                                                 std::vector<AEnemy*>& EnemyVectorPtr,
                                                 std::vector<AKeyObject*>& KeysVectorPtr,
                                                 APlayer& Player,
                                                 ASpriteManager& SpriteManagerPtr,
                                                 AParticleSystemManager& ParticleSystem) const
{
    // Векторы для хранения пуль и врагов для удаления из основных векторов
    std::vector<ABullet*> BulletsToRemove;
    std::vector<APawn*> EnemyToRemove;

    // Перебираем все пули
    for (ABullet* Bullet : BulletsVectorPtr)
    {
        // Проверяем столкновение с игровой картой и её границами 
        if (CheckBulletCollisionWithGameMap(*Bullet))
        {
            // Добавляем пулю на удаление
            BulletsToRemove.emplace_back(Bullet);
        }

        // Проверяем столкновение с персонажем
        if (CheckBulletCollisionWithPawn(*Bullet, Player.GetActorCollisionRect()) &&
            Bullet->GetBulletType() == EBulletType::EBT_ShootAtPlayer)
        {
            // Добавляем пулю на удаление
            BulletsToRemove.emplace_back(Bullet);

            // TODO: Уменьшаем здоровье персонажа, пока он не умрет
            if (PlayerRef.GetPawnMaxHealth() > DEATH)
            {
                PlayerRef.SetPawnCurrentHealth(Bullet->GetBulletDamage());
            }
        }

        // Перебираем всех врагов
        for (APawn* Enemy : EnemyVectorPtr)
        {
            // Проверяем столкновение с врагом
            if (CheckBulletCollisionWithPawn(*Bullet, Enemy->GetActorCollisionRect()) &&
                Bullet->GetBulletType() == EBulletType::EBT_ShootAtEnemy)
            {
                // Добавляем пулю на удаление
                BulletsToRemove.emplace_back(Bullet);

                // Уменьшаем здоровье врага
                Enemy->SetPawnCurrentHealth(Bullet->GetBulletDamage());

                // Добавляем врага на удаление, если его здоровье меньше нуля
                if (Enemy->GetPawnCurrentHealth() <= DEATH)
                {
                    EnemyToRemove.emplace_back(Enemy);
                }
                break;
            }
        }
    }

    // Удаляем пули, столкнувшиеся с препятствиями или врагами
    for (ABullet* BulletRemove : BulletsToRemove)
    {
        // TODO: Выводим информацию для тестирования, удалить позже
        //std::cout << "Delete a bullet at position: " << BulletRemove->GetActorCollisionRect().left << ", "
        //<< BulletRemove->GetActorCollisionRect().top << std::endl;

        std::erase(BulletsVectorPtr, BulletRemove);
        delete BulletRemove;
    }

    // Удаляем врагов, которых убили пули
    for (APawn* EnemyRemove : EnemyToRemove)
    {
        // Проверка, является ли враг боссом
        ABossEnemy* BossEnemy = dynamic_cast<ABossEnemy*>(EnemyRemove);
        if (BossEnemy && BossEnemy->GetPawnCurrentHealth() <= DEATH)
        {
            BossEnemy->OnDeath(KeysVectorPtr, SpriteManagerPtr, BossEnemy->GetActorPosition());
        }

        std::erase(EnemyVectorPtr, EnemyRemove);
        delete EnemyRemove;
    }
}

/**
 * @brief Обрабатывает столкновение пешки с игровой картой.
 * 
 * @param PawnRect Прямоугольник коллизии персонажа.
 * @param ObjectVelocity Вектор скорости объекта персонажа.
 * @param EnemyPtr Указатель на врага.
 * @param bCanJump Может ли персонаж прыгать.
 * @param bCanClimb Может ли персонаж карабкаться.
 */
void ACollisionManager::HandlePawnCollisionWithGameMap(sf::FloatRect& PawnRect,
                                                       sf::Vector2f& ObjectVelocity,
                                                       bool& bCanJump,
                                                       bool& bCanClimb,
                                                       AEnemy* EnemyPtr) const
{
    bCanClimb = false;

    // Проверяем столкнование персонажа с препятствиями игровой карты
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
                // Корректируем положение персонажа по X
                PawnRect.left += FromLeft ? -OverlapLeft : OverlapRight;
                // Останавливаем горизонтальное движение персонажа
                ObjectVelocity.x = 0.f;

                if (EnemyPtr)
                {
                    EnemyPtr->ChangeDirection();
                }
            }
            else // В противном случае это вертикальное столкновение
            {
                // Корректируем положение персонажа по Y
                PawnRect.top += FromTop ? -OverlapTop : OverlapBottom;
                // Останавливаем вертикальное движение персонажа
                ObjectVelocity.y = 0.f;

                // Если персонаж коснулся земли сверху, он может прыгать
                if (FromTop)
                {
                    bCanJump = true;
                }
            }
        }
    }

    // Проверяем, находится ли персонаж на участки карты, который наносит урон
    for (const auto& DamageLayer : GameMapRef.GetDamageCollisionVector())
    {
        if (PawnRect.intersects(DamageLayer))
        {
            // Устанавливаем флаг для персонажа(игрока) на "Умер", если пересекли коллизию с уроном
            bool PlayerDeath = true;
            PlayerRef.SetIsDeathPlayer(PlayerDeath);

            break;
        }
    }


    // Проверяем, находится ли персонаж на лестнице
    for (const auto& LadderLayer : GameMapRef.GetLadderCollisionVector())
    {
        if (PawnRect.intersects(LadderLayer))
        {
            bCanClimb = true;
            break;
        }
    }
}

// TODO: добавить документацию
void ACollisionManager::CheckCollisionWithKey(APlayer& Player, std::vector<AKeyObject*>& KeysVectorPtr)
{
    // Перебираем все ключи на игровой карте
    for (auto it = KeysVectorPtr.begin(); it != KeysVectorPtr.end();)
    {
        AKeyObject* Key = *it;

        // Проверка на столкновение
        if (Player.GetActorCollisionRect().intersects(Key->GetActorCollisionRect()))
        {
            //Key->SetKeyFound(++);

            // Удаление ключа из памяти и вектора
            delete Key;

            // erase возвращает следующий допустимый итератор
            it = KeysVectorPtr.erase(it);
        }
        else
        {
            // Переход к следующему элементу, если не было удаления
            ++it;
        }
    }
}

// void ACollisionManager::CheckCollisionWithChest(APlayer& Player, std::vector<AChestObject*>& ChestVectorPtr)
// {
//     // Перебираем все ключи на игровой карте
//     for (auto it = ChestVectorPtr.begin(); it != ChestVectorPtr.end();)
//     {
//         AChestObject* Chest = *it;
//
//         // Проверка на столкновение
//         if (Player.GetActorCollisionRect().intersects(Chest->GetActorCollisionRect()))
//         {
//             //Key->SetKeyFound(++);
//
//             // Удаление ключа из памяти и вектора
//             delete Chest;
//
//             // erase возвращает следующий допустимый итератор
//             it = ChestVectorPtr.erase(it);
//         }
//         else
//         {
//             // Переход к следующему элементу, если не было удаления
//             ++it;
//         }
//     }
// }

// TODO: добавить документацию
void ACollisionManager::CheckCollisionWithGameMap(sf::FloatRect& ActorRect,
                                                  sf::Vector2f& ObjectVelocity) const
{
    // Проверяем столкнование с препятствиями игровой карты
    for (const auto& Obstacle : GameMapRef.GetGameMapCollisionVector())
    {
        if (ActorRect.intersects(Obstacle))
        {
            // Рассчитываем перекрытие по всем сторонам препятствия
            float OverlapLeft = (ActorRect.left + ActorRect.width) - Obstacle.left;
            float OverlapRight = (Obstacle.left + Obstacle.width) - ActorRect.left;
            float OverlapTop = (ActorRect.top + ActorRect.height) - Obstacle.top;
            float OverlapBottom = (Obstacle.top + Obstacle.height) - ActorRect.top;

            // Определяем направление, откуда пришло столкновение
            bool FromLeft = std::abs(OverlapLeft) < std::abs(OverlapRight);
            bool FromTop = std::abs(OverlapTop) < std::abs(OverlapBottom);

            // Выбираем минимальное перекрытие по X и Y
            float MinOverlapX = FromLeft ? OverlapLeft : OverlapRight;
            float MinOverlapY = FromTop ? OverlapTop : OverlapBottom;

            // Если перекрытие по X меньше, чем по Y, это горизонтальное столкновение
            if (std::abs(MinOverlapX) < std::abs(MinOverlapY))
            {
                // Корректируем положение по X
                ActorRect.left += FromLeft ? -OverlapLeft : OverlapRight;

                // Останавливаем горизонтальное движение
                ObjectVelocity.x = 0.f;
            }
            else // В противном случае это вертикальное столкновение
            {
                // Корректируем положение по Y
                ActorRect.top += FromTop ? -OverlapTop : OverlapBottom;

                // Останавливаем вертикальное движение
                ObjectVelocity.y = 0.f;
            }
        }
    }
}
