#include "GameMap.h"

#include "../Player/Player.h"


AGameMap::AGameMap(APlayer &Player)
    :   PlayerRef(Player)
{
}

void AGameMap::InitGameMap()
{
    // Метод для загрузки карты
    // Проверка загрузки карты, если не удалось - программа завершится с ошибкой
    assert(GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx"));

    // Загрузка текстур tileset'ов
    for (const auto& Tileset : GameMap.getTilesets())
    {
        sf::Texture Texture;
        std::string TexturePath = Tileset.getImagePath();

        // Загрузка текстуры из файла
        Texture.loadFromFile(TexturePath);
        
        // Добавление текстуры в вектор текстур
        TilesetTextures.emplace_back(Texture);
    }

    // Инициализация буфера кадра для карты
    const auto& MapSize = GameMap.getTileCount();
    const auto& TileSize = GameMap.getTileSize();
    MapTexture.create(MapSize.x * TileSize.x, MapSize.y * TileSize.y);

    // Отрисовка всей карты в буфер кадра
    MapTexture.clear();
    for (const auto& Layer : GameMap.getLayers())
    {
        // Проверка типа слоя - интересует только Tile Layer
        if (Layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto& TileLayer = Layer->getLayerAs<tmx::TileLayer>();

            // Здесь DrawScale = 1.0f для оригинального размера
            DrawLayer(TileLayer, MapTexture, 1.0f); 
        }
    }
    MapTexture.display();

    // Создание спрайта для отображения карты
    MapSprite.setTexture(MapTexture.getTexture());
}

sf::IntRect AGameMap::GetTileTextureRect(const tmx::Tileset& Tileset, const uint32_t& TileID)
{
    // Получаем количество столбцов в тайлсете
    uint32_t Columns = Tileset.getColumnCount();
    
    // Получаем размеры тайла
    unsigned TileWidth = Tileset.getTileSize().x;
    unsigned TileHeight = Tileset.getTileSize().y;

    // Локальный ID тайла относительно первого GID в тайлсете
    uint32_t LocalID = TileID - Tileset.getFirstGID();

    // Горизонтальная координата плитки в тайлсете
    uint32_t TileHorizontal = LocalID % Columns;

    // Вертикальная координата плитки в тайлсете
    uint32_t TileVertical = LocalID / Columns;

    // Возвращаем прямоугольник текстуры тайла
    return sf::IntRect(TileHorizontal * TileWidth, TileVertical * TileHeight, TileWidth, TileHeight);
}

void AGameMap::DrawLayer(const tmx::TileLayer& Layer, sf::RenderTarget& Target, float DrawScale)
{
    // Получаем тайлы слоя
    const std::vector<tmx::TileLayer::Tile>& Tiles = Layer.getTiles();
    
    // Получаем размеры карты и тайлов
    const tmx::Vector2u& MapSize = GameMap.getTileCount();
    const tmx::Vector2u& TileSize = GameMap.getTileSize();

    // Создаем массив вершин для отрисовки
    sf::VertexArray Vertices(sf::Quads);
    Vertices.resize(MapSize.x * MapSize.y * 4);

    // Проходимся по всем тайлам слоя
    for (unsigned y = 0; y < MapSize.y; ++y)
    {
        for (unsigned x = 0; x < MapSize.x; ++x)
        {
            // Получаем текущий тайл
            const auto& TileRef = Tiles[y * MapSize.x + x];

            // Пропускаем пустые тайлы
            if (TileRef.ID == 0)
            {
                continue;
            }
            
            // Находим соответствующий tileset для текущего тайла
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
            
            // Пропускаем если тайлсет не найден
            if (!TilesetPtr)
            {
                continue; 
            }
            
            // Получаем текстурный прямоугольник для тайла
            sf::IntRect TextureRect = GetTileTextureRect(*TilesetPtr, TileRef.ID);
            
            // Вычисляем позицию тайла на экране
            sf::Vector2f Position(x * TileSize.x * DrawScale, y * TileSize.y * DrawScale);

            // Указатель на квад из вершин
            sf::Vertex* QuadPtr = &Vertices[(y * MapSize.x + x) * 4];

            // Устанавливаем позиции вершин
            QuadPtr[0].position = sf::Vector2f(Position.x, Position.y);
            QuadPtr[1].position = sf::Vector2f(Position.x + TileSize.x * DrawScale, Position.y);
            QuadPtr[2].position = sf::Vector2f(Position.x + TileSize.x * DrawScale, Position.y + TileSize.y * DrawScale);
            QuadPtr[3].position = sf::Vector2f(Position.x, Position.y + TileSize.y * DrawScale);

            // Устанавливаем текстурные координаты
            QuadPtr[0].texCoords = sf::Vector2f(TextureRect.left, TextureRect.top);
            QuadPtr[1].texCoords = sf::Vector2f(TextureRect.left + TextureRect.width, TextureRect.top);
            QuadPtr[2].texCoords = sf::Vector2f(TextureRect.left + TextureRect.width, TextureRect.top + TextureRect.height);
            QuadPtr[3].texCoords = sf::Vector2f(TextureRect.left, TextureRect.top + TextureRect.height);

            // Отрисовываем квад
            Target.draw(Vertices, &TilesetTextures[TilesetPtr - &GameMap.getTilesets()[0]]);
        }
    }
}

void AGameMap::DrawGameMap(sf::RenderWindow& Window, float DrawScale) const
{
    // Устанавливаем состояния для отрисовки (например, масштабирование)
    sf::RenderStates States;
    States.transform.scale(DrawScale, DrawScale);

    // Отрисовываем спрайт карты
    Window.draw(MapSprite, States);
}
