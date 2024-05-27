#include "GameMap.h"
#include <iostream>


// Конструктор
AGameMap::AGameMap()
{
}

// // Метод для загрузки карты
// void AGameMap::InitGameMap()
// {
//     assert(GameMap.load("D:/VS_Works/GarikMission/GarikMission/Assets/Map/GarikMap.tmx"));
//
//     // D:/VS_Works/GarikMission/GarikMission/Assets/Map/GarikMap.tmx"
//     
//     // Загрузка текстур tileset'ов
//     for (const auto& Tileset : GameMap.getTilesets())
//     {
//         sf::Texture Texture;
//         Texture.loadFromFile(Tileset.getImagePath());
//         TilesetTextures.emplace_back(Texture);
//     }
// }
void AGameMap::InitGameMap()
{
    if (!GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx"))
    {
        std::cerr << "Failed to load map" << std::endl;
        return;
    }

    // Загрузка текстур tileset'ов
    for (const auto& Tileset : GameMap.getTilesets())
    {
        sf::Texture Texture;
        std::string texturePath = Tileset.getImagePath();

        if (!Texture.loadFromFile(texturePath))
        {
            std::cerr << "Failed to load tileset texture: " << texturePath << std::endl;
            return;
        }
        TilesetTextures.push_back(Texture);
    }

    // Инициализация буфера кадра для карты
    const auto& mapSize = GameMap.getTileCount();
    const auto& tileSize = GameMap.getTileSize();
    MapTexture.create(mapSize.x * tileSize.x, mapSize.y * tileSize.y);

    // Отрисовка всей карты в буфер кадра
    MapTexture.clear();
    for (const auto& layer : GameMap.getLayers())
    {
        if (layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
            DrawLayer(tileLayer, MapTexture, 1.0f); // Здесь drawScale = 1.0f для оригинального размера
        }
    }
    MapTexture.display();

    // Создание спрайта для отображения карты
    MapSprite.setTexture(MapTexture.getTexture());
}

sf::IntRect AGameMap::GetTileTextureRect(const tmx::Tileset& Tileset, const std::uint32_t& TileID)
{
    auto columns = Tileset.getColumnCount();
    auto tileWidth = Tileset.getTileSize().x;
    auto tileHeight = Tileset.getTileSize().y;

    std::uint32_t localID = TileID - Tileset.getFirstGID();

    std::uint32_t tu = localID % columns;
    std::uint32_t tv = localID / columns;

    return sf::IntRect(tu * tileWidth, tv * tileHeight, tileWidth, tileHeight);
}

void AGameMap::DrawLayer(const tmx::TileLayer& Layer, sf::RenderTarget& target, float DrawScale)
{
    const auto& tiles = Layer.getTiles();
    const auto& mapSize = GameMap.getTileCount();
    const auto& tileSize = GameMap.getTileSize();

    sf::VertexArray vertices(sf::Quads);
    vertices.resize(mapSize.x * mapSize.y * 4);

    for (unsigned y = 0; y < mapSize.y; ++y)
    {
        for (unsigned x = 0; x < mapSize.x; ++x)
        {
            const auto& tile = tiles[y * mapSize.x + x];
            if (tile.ID == 0) continue;

            const tmx::Tileset* tileset = nullptr;
            for (const auto& ts : GameMap.getTilesets())
            {
                if (tile.ID >= ts.getFirstGID() && tile.ID < ts.getFirstGID() + ts.getTileCount())
                {
                    tileset = &ts;
                    break;
                }
            }
            if (!tileset) continue;

            sf::IntRect textureRect = GetTileTextureRect(*tileset, tile.ID);
            sf::Vector2f position(x * tileSize.x * DrawScale, y * tileSize.y * DrawScale);

            sf::Vertex* quad = &vertices[(y * mapSize.x + x) * 4];

            quad[0].position = sf::Vector2f(position.x, position.y);
            quad[1].position = sf::Vector2f(position.x + tileSize.x * DrawScale, position.y);
            quad[2].position = sf::Vector2f(position.x + tileSize.x * DrawScale, position.y + tileSize.y * DrawScale);
            quad[3].position = sf::Vector2f(position.x, position.y + tileSize.y * DrawScale);

            quad[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
            quad[1].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top);
            quad[2].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height);
            quad[3].texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height);

            target.draw(vertices, &TilesetTextures[tileset - &GameMap.getTilesets()[0]]);
        }
    }
}

void AGameMap::DrawGameMap(sf::RenderWindow& window, float drawScale)
{
    sf::RenderStates states;
    states.transform.scale(drawScale, drawScale);
    window.draw(MapSprite, states);
}
