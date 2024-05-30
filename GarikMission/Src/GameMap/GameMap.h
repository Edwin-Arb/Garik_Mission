#pragma once
#include "../Constants.h"
#include "../Player/Player.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <vector>


class AGameMap
{
public:
    AGameMap(APlayer &Player);
    
    void InitGameMap();
    void DrawGameMap(sf::RenderWindow& Window, float DrawScale) const;

private:
    sf::IntRect GetTileTextureRect(const tmx::Tileset& Tileset, const std::uint32_t& TileID);
    void DrawLayer(const tmx::TileLayer& Layer, sf::RenderTarget& target, float DrawScale);
    
private:
    tmx::Map GameMap;
    sf::Sprite MapSprite;
    sf::RenderTexture MapTexture;
    
    std::vector<sf::Texture> TilesetTextures;
    
    APlayer &PlayerRef;
};
