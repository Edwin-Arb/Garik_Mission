#pragma once
#include "../Constants.h"
#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <vector>

class AGameMap
{
public:
    AGameMap();
    void InitGameMap();
    void DrawGameMap(sf::RenderWindow& Window, float DrawScale);

private:
    tmx::Map GameMap;
    std::vector<sf::Texture> TilesetTextures;
    sf::RenderTexture MapTexture;
    sf::Sprite MapSprite;

    sf::IntRect GetTileTextureRect(const tmx::Tileset& Tileset, const std::uint32_t& TileID);
    void DrawLayer(const tmx::TileLayer& Layer, sf::RenderTarget& target, float DrawScale);
};
