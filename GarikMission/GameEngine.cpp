#include "GameEngine.h"

void AGameEngine::PlayerShoot(std::vector<ABullet*> &BulletsVectorPtr, const sf::FloatRect &PlayerRect)
{
    // Выстрел из оружия. Пока кнопка нажата - мы стреляем 
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        constexpr float SpawnBulletOffsetX = 45.f;
        constexpr float SpawnBulletOffsetY = 20.f;
        BulletsVectorPtr.emplace_back(new ABullet(sf::Vector2f(PlayerRect.left + SpawnBulletOffsetX, PlayerRect.top + SpawnBulletOffsetY)));
    }
}

void AGameEngine::CheckBulletCollision(std::vector<ABullet*> &BulletsVectorPrt, const AEnemy &EnemyCollision, float DeltaTime)
{
    // Вектор для хранения индексов(итераторов) пуль, которые нужно удалить
    std::vector<ABullet*> IndicesToRemove;

    // Проходим по всем пулям в векторе
    for (size_t i = 0; i < BulletsVectorPrt.size(); ++i)
    {        
        // Обновляем позицию пули
        BulletsVectorPrt[i]->UpdateBulletPosition(DeltaTime);
        
        // Проверяем, вышла ли пуля за пределы экрана
        if (BulletsVectorPrt[i]->CheckPositionBulletWithScreen())
        {
            IndicesToRemove.push_back(BulletsVectorPrt[i]);
            continue; // Если пуля выходит за экран, проверка столкновения с врагом не нужна
        }

        // Проверяем столкновение пули с врагом
        if (BulletsVectorPrt[i]->CheckBulletCollisionWithEnemy(EnemyCollision.GetEnemyRect()))
        {
            IndicesToRemove.push_back(BulletsVectorPrt[i]);
        }
    }

    // Удаляем пули, которые встрелись с препятствием, начиная с конца вектора
    for (ABullet *BulletRemove : IndicesToRemove)
    {
        BulletsVectorPrt.erase(std::remove(BulletsVectorPrt.begin(), BulletsVectorPrt.end(), BulletRemove), BulletsVectorPrt.end());
        delete BulletRemove;
    }
}

void AGameEngine::SetSpriteSize(sf::Sprite& Sprite, const float DesiredWidth, const float DesiredHeight)
{// Установить размер спрайта
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();
    const sf::Vector2f Scale = { DesiredWidth / SpriteRect.width, DesiredHeight / SpriteRect.height };
    Sprite.setScale(Scale);
}

void AGameEngine::SetShapeSize(sf::Shape& Shape, const float DesiredWidth, const float DesiredHeight)
{// Установить размер формы(shape)
    const sf::FloatRect SpriteRect = Shape.getLocalBounds();
    const sf::Vector2f Scale = { DesiredWidth / SpriteRect.width, DesiredHeight / SpriteRect.height };
    Shape.setScale(Scale);
}

void AGameEngine::SetSpriteRelativeOrigin(sf::Sprite& Sprite, const float OriginX, const float OriginY)
{// Установить центр координа для спрайта(тексты объекта)
    const sf::FloatRect SpriteRect = Sprite.getLocalBounds();
    Sprite.setOrigin(OriginX * SpriteRect.width, OriginY * SpriteRect.height);
}

void AGameEngine::SetShapeRelativeOrigin(sf::Shape& Shape, const float OriginX, const float OriginY)
{// Установить центр координа для формы(коллизии)
    const sf::FloatRect ShapeRect = Shape.getLocalBounds();
    Shape.setOrigin(OriginX * ShapeRect.width, OriginY * ShapeRect.height);
}



