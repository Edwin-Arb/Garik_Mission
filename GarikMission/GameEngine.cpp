#include "GameEngine.h"

// AGameEngine::AGameEngine()
// {
//     player = new APlayer;
// }
//
// AGameEngine::~AGameEngine()
// {
//     delete player;
// }

bool AGameEngine::DoShapeCollide(const Rectangle& rect1, const Rectangle& rect2)
{
    return (rect1.position2D.X < rect2.position2D.X + rect2.size.X) &&
           (rect1.position2D.X + rect1.size.X > rect2.position2D.X) &&
           (rect1.position2D.Y < rect2.position2D.Y + rect2.size.Y) &&
           (rect1.position2D.Y + rect1.size.Y > rect2.position2D.Y);
}

// void AGameEngine::DrawGame(sf::RenderWindow &window)
// {
//     player->DrawPlayer(window);
// }
