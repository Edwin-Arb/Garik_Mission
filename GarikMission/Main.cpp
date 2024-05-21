#include "Constants.h"
#include "GameState.h"
// #include <tmxlite/Map.hpp>
// #include <tmxlite/Layer.hpp>
// #include <tmxlite/TileLayer.hpp>
// #include <tmxlite/Tileset.hpp>
// #include <vector>
// #include <iostream>


// bool loadMap(const std::string& filepath, tmx::Map& map)
// {
//     if (!map.load(filepath))
//     {
//         std::cerr << "Failed to load map: " << filepath << std::endl;
//         return false;
//     }
//     return true;
// }
//
// sf::Texture loadTilesetTexture(const std::string& imagePath)
// {
//     sf::Texture texture;
//     if (!texture.loadFromFile(imagePath))
//     {
//         std::cerr << "Failed to load tileset texture: " << imagePath << std::endl;
//     }
//     return texture;
// }
//
// sf::IntRect getTileTextureRect(const tmx::Tileset& tileset, std::uint32_t tileID)
// {
//     auto columns = tileset.getColumnCount();
//     auto tileWidth = tileset.getTileSize().x;
//     auto tileHeight = tileset.getTileSize().y;
//
//     std::uint32_t localID = tileID - tileset.getFirstGID();
//
//     std::uint32_t tu = localID % columns;
//     std::uint32_t tv = localID / columns;
//
//     return sf::IntRect(tu * tileWidth, tv * tileHeight, tileWidth, tileHeight);
// }
//
// void drawLayer(const tmx::TileLayer& layer, const tmx::Map& map, sf::RenderWindow& window, const std::vector<sf::Texture>& tilesetTextures)
// {
//     const auto& tiles = layer.getTiles();
//     const auto& mapSize = map.getTileCount();
//     const auto& tileSize = map.getTileSize();
//
//     sf::VertexArray vertices(sf::Quads);
//     vertices.resize(mapSize.x * mapSize.y * 4);
//
//     for (unsigned y = 0; y < mapSize.y; ++y)
//     {
//         for (unsigned x = 0; x < mapSize.x; ++x)
//         {
//             const auto& tile = tiles[y * mapSize.x + x];
//             if (tile.ID == 0) continue;
//
//             // Найдем tileset, к которому принадлежит этот тайл
//             const tmx::Tileset* tileset = nullptr;
//             for (const auto& ts : map.getTilesets())
//             {
//                 if (tile.ID >= ts.getFirstGID() && tile.ID < ts.getFirstGID() + ts.getTileCount())
//                 {
//                     tileset = &ts;
//                     break;
//                 }
//             }
//             if (!tileset) continue;
//
//             sf::IntRect textureRect = getTileTextureRect(*tileset, tile.ID);
//             sf::Vector2f position(x * tileSize.x, y * tileSize.y);
//
//             sf::Vertex* quad = &vertices[(y * mapSize.x + x) * 4];
//
//             quad[0].position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
//             quad[1].position = sf::Vector2f((x + 1) * tileSize.x, y * tileSize.y);
//             quad[2].position = sf::Vector2f((x + 1) * tileSize.x, (y + 1) * tileSize.y);
//             quad[3].position = sf::Vector2f(x * tileSize.x, (y + 1) * tileSize.y);
//
//             quad[0].texCoords = sf::Vector2f(textureRect.left, textureRect.top);
//             quad[1].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top);
//             quad[2].texCoords = sf::Vector2f(textureRect.left + textureRect.width, textureRect.top + textureRect.height);
//             quad[3].texCoords = sf::Vector2f(textureRect.left, textureRect.top + textureRect.height);
//
//             // Убедитесь, что используете правильную текстуру для отрисовки
//             window.draw(vertices, &tilesetTextures[tileset - &map.getTilesets()[0]]);
//         }
//     }
// }

int main()
{
    // tmx::Map map;
    // if (!loadMap("D:/VS_Works/GarikMission/GarikMission/Resources/Map/GarikMap.tmx", map))
    // {
    //     std::cerr << "Failed to load map!" << std::endl;
    //     return -1;
    // }

    // // Load the tileset textures
    // std::vector<sf::Texture> tilesetTextures;
    // for (const auto& tileset : map.getTilesets())
    // {
    //     tilesetTextures.push_back(loadTilesetTexture(tileset.getImagePath()));
    // }
    
    sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Garik's Mission");

    sf::Clock gameClock;
    
    AGameState game;
    game.InitGame();
    
    while (window.isOpen())
    {
        sf::sleep(sf::milliseconds(1.f));
        float deltaTime = gameClock.getElapsedTime().asSeconds();
        gameClock.restart();
        
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            //game.HandleUserInput(event);
        }

        // Update Game State
        game.UpdateGameplay(event, deltaTime);
        
        // Draw Game
        window.clear();

        // // Render all tile layers of the map
        // for (const auto& layer : map.getLayers())
        // {
        //     if (layer->getType() == tmx::Layer::Type::Tile)
        //     {
        //         const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        //         drawLayer(tileLayer, map, window, tilesetTextures);
        //     }
        // }
        
        game.DrawGame(window);
        
        window.display();
    }
    
    return 0;
}
