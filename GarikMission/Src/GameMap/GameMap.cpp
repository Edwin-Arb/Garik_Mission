#include "GameMap.h"
#include <iostream>

/**
 * @brief Конструктор класса AGameMap.
 *
 * @param Player Ссылка на объект игрока.
 */
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
    GameMapCollisionLayer.reserve(ReserveCollisionCapacity); // Резервируем память для коллизионного слоя
    LadderCollisionLayer.reserve(5); // Резервируем память для слоя лестниц

    // Загрузка карты
    assert(GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx")); // Проверяем успешную загрузку карты
    LoadTilesets(GameMap); // Загружаем тайлсеты карты
    CheckLayers(GameMap); // Проверяем слои карты
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
        if (Texture.loadFromFile(Tileset.getImagePath()))
        {
            TilesetTextures[Tileset.getFirstGID()] = std::move(Texture); // Сохраняем текстуры тайлсетов
        }
        else
        {
            std::cerr << "Error download a texture: " << Tileset.getImagePath() << std::endl;
            // Выводим ошибку загрузки текстуры
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
            ProcessTileLayer(dynamic_cast<const tmx::TileLayer*>(Layer.get())); // Обрабатываем тайловый слой
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
            uint32_t TextureHorizontal = LocalTileID % (CurrentTexturePtr->getSize().x / 16);
            uint32_t TextureVertical = LocalTileID / (CurrentTexturePtr->getSize().x / 16);

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

            Quad[0].texCoords = sf::Vector2f(TextureHorizontal * 16.f, TextureVertical * 16.f);
            Quad[1].texCoords = sf::Vector2f((TextureHorizontal + 1) * 16.f, TextureVertical * 16.f);
            Quad[2].texCoords = sf::Vector2f((TextureHorizontal + 1) * 16.f, (TextureVertical + 1) * 16.f);
            Quad[3].texCoords = sf::Vector2f(TextureHorizontal * 16.f, (TextureVertical + 1) * 16.f);
        }
    }

    for (const auto& Pair : LayerVertexMap)
    {
        LayersVector.emplace_back(Pair.second); // Добавляем вершины слоя в вектор слоев
        RenderStatesVector.emplace_back(); // Добавляем RenderStates для каждого слоя
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
                    GameMapCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height); // Добавляем коллизии в вектор
                }
            }
            else if (ObjectLayer && ObjectLayer->getName() == "Ladders")
            {
                // Обработка лестниц
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    tmx::FloatRect TmxRect = Object.getAABB();
                    LadderCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height); // Добавляем лестницы в вектор
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
 * @param View Ссылка на вид SFML для установки вида карты.
 */
void AGameMap::DrawGameMap(sf::RenderWindow& Window, const sf::View &View) const
{
    // Отрисовка карты    
    for (size_t i = 0; i < LayersVector.size(); ++i)
    {
        Window.draw(LayersVector[i], RenderStatesVector[i]);
    }
}