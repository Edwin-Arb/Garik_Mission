#include "GameMap.h"
#include <iostream>

/**
 * @brief Конструктор класса AGameMap.
 *
 * @param Player Ссылка на объект игрока.
 */
// AGameMap::AGameMap(APlayer& Player)
//     : PlayerRef(Player)
// {
// }

AGameMap::AGameMap(APlayer& Player)
    : PlayerRef(Player)
{
}

/**
 * @brief Деструктор класса AGameMap.
 */
AGameMap::~AGameMap()
{
    // Очищаем контейнер с текстурами
    TilesetTextures.clear();

    // Очистка векторов
    LayersVector.clear();
    RenderStatesVector.clear();
    GameMapCollisionLayer.clear();
    LadderCollisionLayer.clear();
    SpawnBaseEnemyPosition.clear();
}

/**
 * @brief Инициализация игровой карты.
 *
 * Метод загружает карту, тайлсеты и настраивает слои коллизий и лестницы.
 */
void AGameMap::InitGameMap()
{
    // TODO: Изменить в будущем значение, на фактическое количество коллизий карты и лестниц в игре
    int ReserveCollisionCapacity = 100;

    // Резервируем память для коллизионного слоя
    GameMapCollisionLayer.reserve(ReserveCollisionCapacity);

    // Резервируем память для слоя лестниц
    LadderCollisionLayer.reserve(5);

    // Загрузка карты
    // Проверяем успешную загрузку карты
    assert(GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx"));

    // Загружаем тайлсеты карты
    LoadTilesets(GameMap);

    // Проверяем слои карты
    CheckLayers(GameMap);
    ProcessCollisionLayers(GameMap);
}

/**
 * @brief Загрузка текстур тайлсетов карты.
 *
 * @param GameMap Константная ссылка на объект карты TMX.
 */
void AGameMap::LoadTilesets(const tmx::Map& GameMap)
{
    // Загрузка текстур тайлсетов
    const auto& Tilesets = GameMap.getTilesets();
    for (const auto& Tileset : Tilesets)
    {
        sf::Texture Texture;
        std::string ImagePath = Tileset.getImagePath();

        if (Texture.loadFromFile(Tileset.getImagePath()))
        {
            // Сохраняем текстуры тайлсетов
            TilesetTextures[Tileset.getFirstGID()] = std::move(Texture);
        }
        else
        {
            // Выводим ошибку загрузки текстуры
            std::cerr << "Error loading texture: " << ImagePath << std::endl;
        }
    }
}

/**
 * @brief Проверка и обработка тайловых слоев карты.
 *
 * @param GameMap Константная ссылка на объект карты TMX.
 */
void AGameMap::CheckLayers(const tmx::Map& GameMap)
{
    // Проверка и обработка тайловых слоев
    for (const auto& Layer : GameMap.getLayers())
    {
        if (Layer->getType() == tmx::Layer::Type::Tile)
        {
            // Обрабатываем тайловый слой
            ProcessTileLayer(dynamic_cast<const tmx::TileLayer*>(Layer.get()));
        }
    }
}

/**
 * @brief Обработка тайлового слоя карты.
 *
 * @param TileLayerPtr Указатель на тайловый слой карты TMX.
 */
void AGameMap::ProcessTileLayer(const tmx::TileLayer* TileLayerPtr)
{
    // Обработка тайлового слоя
    const auto& Tiles = TileLayerPtr->getTiles();
    const auto& MapSize = TileLayerPtr->getSize();
    std::map<const sf::Texture*, sf::VertexArray> LayerVertexMap;

    for (uint32_t y = 0; y < MapSize.y; ++y)
    {
        for (uint32_t x = 0; x < MapSize.x; ++x)
        {
            tmx::TileLayer::Tile Tile = Tiles[y * MapSize.x + x];

            if (Tile.ID == 0)
            {
                continue; // Пропускаем пустые тайлы
            }

            const sf::Texture* CurrentTexturePtr = nullptr;
            uint32_t FirstGID = 0;
            for (const auto& Pair : TilesetTextures)
            {
                if (Tile.ID >= Pair.first)
                {
                    CurrentTexturePtr = &Pair.second;
                    FirstGID = Pair.first;
                }
                else
                {
                    break;
                }
            }

            if (!CurrentTexturePtr)
            {
                continue; // Пропускаем, если текстура не найдена
            }

            uint32_t LocalTileID = Tile.ID - FirstGID;
            uint32_t tilesPerRow = CurrentTexturePtr->getSize().x / 16; // Количество тайлов в строке
            uint32_t TextureHorizontal = LocalTileID % tilesPerRow;
            uint32_t TextureVertical = LocalTileID / tilesPerRow;

            sf::VertexArray& Vertices = LayerVertexMap[CurrentTexturePtr];
            if (Vertices.getVertexCount() == 0)
            {
                Vertices.setPrimitiveType(sf::Quads);
                Vertices.resize(MapSize.x * MapSize.y * 4);
            }

            sf::Vertex* Quad = &Vertices[(x + y * MapSize.x) * 4];

            Quad[0].position = sf::Vector2f(x * 16.f, y * 16.f);
            Quad[1].position = sf::Vector2f((x + 1) * 16.f, y * 16.f);
            Quad[2].position = sf::Vector2f((x + 1) * 16.f, (y + 1) * 16.f);
            Quad[3].position = sf::Vector2f(x * 16.f, (y + 1) * 16.f);

            sf::Vector2f texCoords[4] = {
                sf::Vector2f(TextureHorizontal * 16.f, TextureVertical * 16.f),
                sf::Vector2f((TextureHorizontal + 1) * 16.f, TextureVertical * 16.f),
                sf::Vector2f((TextureHorizontal + 1) * 16.f, (TextureVertical + 1) * 16.f),
                sf::Vector2f(TextureHorizontal * 16.f, (TextureVertical + 1) * 16.f)
            };

            Quad[0].texCoords = texCoords[0];
            Quad[1].texCoords = texCoords[1];
            Quad[2].texCoords = texCoords[2];
            Quad[3].texCoords = texCoords[3];
        }
    }

    for (const auto& Pair : LayerVertexMap)
    {
        LayersVector.emplace_back(Pair.second);
        RenderStatesVector.emplace_back();
        RenderStatesVector.back().texture = Pair.first;
    }
}

/**
 * @brief Обработка объектных слоев карты для получения коллизий и лестниц.
 *
 * @param GameMap Константная ссылка на объект карты TMX.
 */
void AGameMap::ProcessCollisionLayers(const tmx::Map& GameMap)
{
    // Обработка объектных слоев

    for (const auto& Layer : GameMap.getLayers())
    {
        if (Layer->getType() == tmx::Layer::Type::Object)
        {
            auto* ObjectLayer = dynamic_cast<const tmx::ObjectGroup*>(Layer.get());
            if (ObjectLayer && ObjectLayer->getName() == "Obstacles")
            {
                // Обработка коллизий
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    tmx::FloatRect TmxRect = Object.getAABB();
                    GameMapCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                    // Добавляем коллизии в вектор
                }
            }
            else if (ObjectLayer && ObjectLayer->getName() == "Ladders")
            {
                // Обработка лестниц
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    tmx::FloatRect TmxRect = Object.getAABB();
                    LadderCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                    // Добавляем лестницы в вектор
                }
            }
            else if (ObjectLayer && ObjectLayer->getName() == "SpawnEnemy")
            {
                // Обработка лестниц
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    sf::Vector2f TmxRect = {Object.getPosition().x, Object.getPosition().y};

                    // TODO: Сделать в Tiled отдельный спавн слой для боссов
                    if (Object.getUID() == 399)
                    {
                        // Добавляем Босс-врагов в вектор
                        SpawnBossEnemyPosition.emplace_back(TmxRect);
                    }
                    else
                    {
                        SpawnBaseEnemyPosition.emplace_back(TmxRect);
                    }
                }
            }
        }
    }
}

/**
 * @brief Получение вектора коллизий игровой карты.
 *
 * @return Вектор коллизий игровой карты.
 */
std::vector<sf::FloatRect> AGameMap::GetGameMapCollisionVector() const
{
    return GameMapCollisionLayer;
}

/**
 * @brief Получение вектора лестниц игровой карты.
 *
 * @return Вектор лестниц игровой карты.
 */
std::vector<sf::FloatRect> AGameMap::GetLadderCollisionVector() const
{
    return LadderCollisionLayer;
}

/**
 * @brief Отрисовка игровой карты на окне.
 *
 * @param Window Ссылка на окно SFML для отрисовки.
 */
void AGameMap::DrawGameMap(sf::RenderWindow& Window) const
{
    // Отрисовка карты    
    for (size_t i = 0; i < LayersVector.size(); ++i)
    {
        Window.draw(LayersVector[i], RenderStatesVector[i]);
    }
}
