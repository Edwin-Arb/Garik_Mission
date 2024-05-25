#include "InputManager.h"

void AInputManager::PlayerShoots(std::vector<ABullet*> &BulletsVectorPtr, const sf::FloatRect &PlayerRect, IRendererSprite &Renderer)
{
    // Выстрел из оружия. Пока кнопка нажата - мы стреляем 
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        constexpr float SpawnBulletOffsetX = 50.f;
        constexpr float SpawnBulletOffsetY = 20.f;
        BulletsVectorPtr.emplace_back(new ABullet(
                                       sf::Vector2f(PlayerRect.left + SpawnBulletOffsetX,
                                                 PlayerRect.top + SpawnBulletOffsetY),
                                              Renderer));
    }
}

void AInputManager::CheckBulletCollision(std::vector<ABullet*> &BulletsVectorPrt, const AEnemy &EnemyCollision, float DeltaTime)
{
    // Вектор для хранения индексов(итераторов) пуль, которые нужно удалить
    std::vector<ABullet*> IndicesToRemove;

    // Проходим по всем пулям в векторе
    for (ABullet *Bullet : BulletsVectorPrt)
    {        
        // Обновляем позицию пули
        Bullet->UpdateBulletPosition(DeltaTime);
        
        // Проверяем, вышла ли пуля за пределы экрана
        if (Bullet->CheckPositionBulletWithScreen())
        {
            IndicesToRemove.push_back(Bullet);
            continue; // Если пуля выходит за экран, проверка столкновения с врагом не нужна
        }

        // Проверяем столкновение пули с врагом
        if (Bullet->CheckBulletCollisionWithEnemy(EnemyCollision.GetEnemyRect()))
        {
            IndicesToRemove.push_back(Bullet);
        }
    }

    // Удаляем пули, которые встрелись с препятствием, начиная с конца вектора
    for (ABullet *BulletRemove : IndicesToRemove)
    {
        BulletsVectorPrt.erase(std::remove(BulletsVectorPrt.begin(), BulletsVectorPrt.end(), BulletRemove), BulletsVectorPrt.end());
        delete BulletRemove;
    }
}