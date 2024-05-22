#include "GameEngine.h"
#include "GameState.h"

// AGameEngine::AGameEngine()
// {
//     gamePtr = new AGameState;
// }
//
// AGameEngine::~AGameEngine()
// {
//     delete gamePtr;
// }

bool AGameEngine::DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2)
{
    return (rect1.position2D.X < rect2.position2D.X + rect2.size.X) &&
           (rect1.position2D.X + rect1.size.X > rect2.position2D.X) &&
           (rect1.position2D.Y < rect2.position2D.Y + rect2.size.Y) &&
           (rect1.position2D.Y + rect1.size.Y > rect2.position2D.Y);
}

void AGameEngine::PlayerShoot(std::vector<ABullet> &bullets_vector, const sf::FloatRect &player_rect)
{
    // Выстрел из оружия. Пока кнопка нажата - мы стреляем 
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        constexpr float spawn_bullet_offsetX = 90.f;
        constexpr float spawn_bullet_offsetY = 75.f;
        bullets_vector.emplace_back(sf::Vector2f(player_rect.left + spawn_bullet_offsetX, player_rect.top + spawn_bullet_offsetY));
    }
}

void AGameEngine::CheckBulletCollision(std::vector<ABullet> &bullets_vector, const AEnemy &enemy_collision, float delta_time)
{
    // Обновление передвижения пули
    std::vector<size_t> indices_to_remove;

    for (size_t i = 0; i < bullets_vector.size(); ++i)
    {
        bullets_vector[i].UpdateBulletPosition(delta_time);
        if (bullets_vector[i].CheckPositionBulletWithScreen())
        {
            indices_to_remove.push_back(i);
        }

        if (bullets_vector[i].CheckBulletCollisionWithEnemy(enemy_collision.GetEnemyRect()))
        {
            indices_to_remove.push_back(i);
        }
    }

    // Удаляем пули, которые встрелись с препятствием
    for (auto it = indices_to_remove.rbegin(); it != indices_to_remove.rend(); ++it)
    {
        bullets_vector.erase(bullets_vector.begin() + static_cast<size_t>(*it));
    }
}

