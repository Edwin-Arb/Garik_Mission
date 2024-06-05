#include "GameMap.h"
#include <iostream>

AGameMap::AGameMap(APlayer& Player)
    : PlayerRef(Player)
{
    // Устанавливаем тип примитивов в массиве вершин
    Vertices.setPrimitiveType(sf::Quads);
}

// Инициализация карты игры
void AGameMap::InitGameMap()
{
    // Загрузка карты из файла
    assert(GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx"));

    // Загрузка тайлсетов
    for (const auto& Tileset : GameMap.getTilesets())
    {
        sf::Texture Texture;
        std::string TexturePath = Tileset.getImagePath();
        if (!Texture.loadFromFile(TexturePath))
        {
            // Обработка ошибки загрузки текстуры
            std::cerr << "Failed to load texture: " << TexturePath << std::endl;
            continue;
        }
        TilesetTextures.emplace_back(Texture);
    }

    // Получение размеров карты и тайлов
    const tmx::Vector2u& MapSize = GameMap.getTileCount();
    const tmx::Vector2u& TileSize = GameMap.getTileSize();
    MapTexture.create(MapSize.x * TileSize.x, MapSize.y * TileSize.y);

    // Очистка текстуры карты перед рисованием
    MapTexture.clear();
    for (const auto& Layer : GameMap.getLayers())
    {
        std::cout << "Layer type: " << static_cast<int>(Layer->getType()) << ", Layer name: " << Layer->getName() << std::endl;

        // Отрисовка тайл-слоя
        if (Layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto& TileLayer = Layer->getLayerAs<tmx::TileLayer>();
            DrawLayer(TileLayer, MapTexture, 1.0f);
        }

        // Обработка слоя объектов
        if (Layer->getType() == tmx::Layer::Type::Object)
        {
            auto* objectLayer = dynamic_cast<const tmx::ObjectGroup*>(Layer.get());
            std::cout << "Object Layer name: " << objectLayer->getName() << std::endl;

            if (objectLayer && objectLayer->getName() == "Obstacles")
            {
                std::cout << "Found Obstacles layer" << std::endl;

                // Добавление препятствий в вектор коллизий
                for (const auto& object : objectLayer->getObjects())
                {
                    tmx::FloatRect tmxRect = object.getAABB();
                    sf::FloatRect sfRect(tmxRect.left, tmxRect.top, tmxRect.width, tmxRect.height);
                    GetCollisionLayer.emplace_back(sfRect);
                }
            }
        }
    }
    // Завершение рисования и отображение текстуры карты
    MapTexture.display();
    MapSprite.setTexture(MapTexture.getTexture());
    std::cout << "Number of objects in 'Obstacles' layer: " << GetCollisionLayer.size() << std::endl;
}

// Проверка коллизии игрока с препятствиями
bool AGameMap::CheckCollision(const sf::FloatRect& PlayerBounds) const
{
    for (const auto& obstacle : GetCollisionLayer)
    {
        if (PlayerBounds.intersects(obstacle))
        {
            return true;
        }
    }
    return false;
}

// Получение текстурных координат тайла по его ID
sf::IntRect AGameMap::GetTileTextureRect(const tmx::Tileset& Tileset, const uint32_t& TileID) const
{
    uint32_t Columns = Tileset.getColumnCount();
    unsigned TileWidth = Tileset.getTileSize().x;
    unsigned TileHeight = Tileset.getTileSize().y;
    uint32_t LocalID = TileID - Tileset.getFirstGID();
    uint32_t TileHorizontal = LocalID % Columns;
    uint32_t TileVertical = LocalID / Columns;
    return sf::IntRect(TileHorizontal * TileWidth, TileVertical * TileHeight, TileWidth, TileHeight);
}

// Отрисовка слоя тайлов
void AGameMap::DrawLayer(const tmx::TileLayer& Layer, sf::RenderTarget& Target, float DrawScale)
{
    const std::vector<tmx::TileLayer::Tile>& Tiles = Layer.getTiles();
    const tmx::Vector2u& MapSize = GameMap.getTileCount();
    const tmx::Vector2u& TileSize = GameMap.getTileSize();

    // Очистка и настройка массива вершин
    Vertices.clear();
    Vertices.resize(MapSize.x * MapSize.y * 4);

    for (unsigned y = 0; y < MapSize.y; ++y)
    {
        for (unsigned x = 0; x < MapSize.x; ++x)
        {
            const auto& TileRef = Tiles[y * MapSize.x + x];
            if (TileRef.ID == 0)
            {
                continue;
            }

            const tmx::Tileset* TilesetPtr = nullptr;
            for (const tmx::Tileset& Tileset : GameMap.getTilesets())
            {
                if (TileRef.ID >= Tileset.getFirstGID() && TileRef.ID <
                    Tileset.getFirstGID() + Tileset.getTileCount())
                {
                    TilesetPtr = &Tileset;
                    break;
                }
            }

            if (!TilesetPtr)
            {
                continue;
            }

            sf::IntRect TextureRect = GetTileTextureRect(*TilesetPtr, TileRef.ID);
            sf::Vector2f Position(x * TileSize.x * DrawScale, y * TileSize.y * DrawScale);
            sf::Vertex* QuadPtr = &Vertices[(y * MapSize.x + x) * 4];
            QuadPtr[0].position = sf::Vector2f(Position.x, Position.y);
            QuadPtr[1].position = sf::Vector2f(Position.x + TileSize.x * DrawScale, Position.y);
            QuadPtr[2].position = sf::Vector2f(Position.x + TileSize.x * DrawScale,
                                               Position.y + TileSize.y * DrawScale);
            QuadPtr[3].position = sf::Vector2f(Position.x, Position.y + TileSize.y * DrawScale);

            QuadPtr[0].texCoords = sf::Vector2f(TextureRect.left, TextureRect.top);
            QuadPtr[1].texCoords = sf::Vector2f(TextureRect.left + TextureRect.width, TextureRect.top);
            QuadPtr[2].texCoords = sf::Vector2f(TextureRect.left + TextureRect.width,
                                                TextureRect.top + TextureRect.height);
            QuadPtr[3].texCoords = sf::Vector2f(TextureRect.left, TextureRect.top + TextureRect.height);
        }
    }

    // Отрисовка массива вершин с использованием текстуры тайлсета
    for (const auto& Tileset : GameMap.getTilesets())
    {
        Target.draw(Vertices, &TilesetTextures[Tileset.getFirstGID() - 1]);
    }
}

// Отрисовка всей карты
void AGameMap::DrawGameMap(sf::RenderWindow& Window, float DrawScale) const
{
    sf::RenderStates States;
    States.transform.scale(DrawScale, DrawScale);
    Window.draw(MapSprite, States);
}

// Получение вектора препятствий для коллизий
std::vector<sf::FloatRect> AGameMap::GetCollisionVector() const
{
    return GetCollisionLayer;
}