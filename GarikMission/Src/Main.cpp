#include <iostream>
#include "GameMain/GameState.h"

// #include <tmxlite/Map.hpp>
// #include <tmxlite/Layer.hpp>
// #include <tmxlite/TileLayer.hpp>
// #include <tmxlite/Tileset.hpp>
// #include <vector>
// #include <iostream>
//
//
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
    // if (!loadMap("D:/VS_Works/GarikMission/GarikMission/Assets/Map/GarikMap.tmx", map))
    // {
    //     std::cerr << "Failed to load map!" << std::endl;
    //     return -1;
    // }
    //
    // // Load the tileset textures
    // std::vector<sf::Texture> tilesetTextures;
    // for (const auto& tileset : map.getTilesets())
    // {
    //     tilesetTextures.push_back(loadTilesetTexture(tileset.getImagePath()));
    // }
    
    sf::RenderWindow Window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Garik Mission");
    //Window.setFramerateLimit(500);

    // Делаем отображение количества кадров в секунду(FPS)
    sf::Text FPSText;
    sf::Font FPSFont;
    assert(FPSFont.loadFromFile(RESOURCES_PATH + "Fonts/Roboto-Bold.ttf"));
    FPSText.setFont(FPSFont);
    FPSText.setCharacterSize(15);
    FPSText.setPosition(10.f, 10.f);
    
    sf::Clock GameClock;
        
    AGameState Game;
    Game.InitGame();
    
    while (Window.isOpen())
    {
        // Делаем задержку между кадрами, чтобы игра работала на всех компьютерах одинаково
        sleep(sf::milliseconds(8));
        float DeltaTime = GameClock.getElapsedTime().asSeconds();
        
        // Текст, который показывает количества кадров в секунду(FPS)
        FPSText.setString("FPS: " + std::to_string(static_cast<int>(1/DeltaTime)));

        std::cout << DeltaTime << "\n"; // TODO для тестов
        
        // Обновляем таймер между кадрами
        GameClock.restart();
        
        // Проверяем ивенты во время открытия окна
        sf::Event event;
        while (Window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                Window.close();
            }
        }

        // Проверяем какие клавиши нажал Игрок
        Game.UpdateInput(DeltaTime);
       
        // Обновлять игровое состояние
        Game.UpdateGameplay(DeltaTime);
        
        // Очищать экран
        Window.clear();

        // // Render all tile layers of the map
        // for (const auto& layer : map.getLayers())
        // {
        //     if (layer->getType() == tmx::Layer::Type::Tile)
        //     {
        //         const auto& tileLayer = layer->getLayerAs<tmx::TileLayer>();
        //         drawLayer(tileLayer, map, Window, tilesetTextures);
        //     }
        // }
        
        // Отрисовываем игру
        Game.DrawGame(Window);
        Window.draw(FPSText);
        
        Window.display();
    }
    
    return 0;
}
