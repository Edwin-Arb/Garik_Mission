#pragma once
#include "Constants.h"
#include "Bullet.h"
#include "Enemy.h"


class AGameEngine
{
public:
    // AGameEngine();
    // ~AGameEngine();

    
    
    static bool DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2);
    static void PlayerShoot(std::vector<ABullet>& bullets_vector, const sf::FloatRect& player_rect);
    static void CheckBulletCollision(std::vector<ABullet>& bullets_vector, const AEnemy& enemy_collision, float delta_time);
    //void DrawGame(sf::RenderWindow& window);

private:
    //AGameState gamePtr;
    // AGameState game;
    // APlayer *player = new APlayer;
};
