#include "TileAnimator.h"
#include <iostream>
#include "GameMap.h"


ATileAnimator::ATileAnimator(AGameMap* GameMapPtr)
    : GameMapPtr(GameMapPtr)
{
}

ATileAnimator::~ATileAnimator()
{
}

void ATileAnimator::Update(float DeltaTime)
{
    for (auto& [TileID, Timer] : AnimationTimers)
    {
        if (GameMapPtr->AnimatedTiles.find(TileID) != GameMapPtr->AnimatedTiles.end())
        {
            SAnimatedTile& AnimTile = GameMapPtr->AnimatedTiles[TileID];
            Timer += DeltaTime;
            if (Timer >= AnimTile.FrameDurations[AnimTile.CurrentFrame])
            {
                Timer = 0.f;
                AnimTile.CurrentFrame = (AnimTile.CurrentFrame + 1) % AnimTile.FrameDurations.size();
                //std::cout << "TileID: " << TileID << " CurrentFrame: " << AnimTile.CurrentFrame << std::endl;
            }
        }
    }
}

void ATileAnimator::DrawTileAnimator(sf::RenderWindow& Window) const
{
    for (const auto& [TileID, AnimTile] : GameMapPtr->AnimatedTiles)
    {
        sf::Sprite sprite;
        uint32_t CurrentFrameID = AnimTile.FrameIDs[AnimTile.CurrentFrame];

        const sf::Texture* CurrentTexturePtr = nullptr;
        uint32_t FirstGID = 0;
        for (const auto& Pair : GameMapPtr->TilesetTextures)
        {
            if (CurrentFrameID >= Pair.first)
            {
                CurrentTexturePtr = &Pair.second;
                FirstGID = Pair.first;
            }
            else
            {
                break;
            }
        }

        if (CurrentTexturePtr)
        {
            uint32_t LocalTileID = CurrentFrameID - FirstGID;
            uint32_t tilesPerRow = CurrentTexturePtr->getSize().x / 16;
            uint32_t TextureHorizontal = LocalTileID % tilesPerRow;
            uint32_t TextureVertical = LocalTileID / tilesPerRow;

            // TODO: Debug information
            // std::cout << "CurrentFrameID: " << CurrentFrameID << "\n";
            // std::cout << "FirstGID: " << FirstGID << "\n";
            // std::cout << "LocalTileID: " << LocalTileID << "\n";
            // std::cout << "TextureHorizontal: " << TextureHorizontal << "\n";
            // std::cout << "TextureVertical: " << TextureVertical << "\n";

            sprite.setTexture(*CurrentTexturePtr);
            sprite.setTextureRect(sf::IntRect(TextureHorizontal * 16, TextureVertical * 16, 16, 16));
            sprite.setPosition(AnimTile.Position);

            Window.draw(sprite);
        }
    }
}


uint32_t ATileAnimator::GetCurrentFrame(uint32_t TileID) const
{
    if (GameMapPtr->AnimatedTiles.find(TileID) != GameMapPtr->AnimatedTiles.end())
    {
        return GameMapPtr->AnimatedTiles[TileID].FrameIDs[GameMapPtr->AnimatedTiles[TileID].CurrentFrame];
    }
    return TileID;
}
