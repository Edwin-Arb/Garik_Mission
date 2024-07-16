// GameMap.cpp

#include "GameMap.h"

#include <iostream>
#include <tmxlite/TileLayer.hpp>

AGameMap::AGameMap(APlayer& Player)
    : PlayerRef(Player), TileAnimator(this)
{
}

AGameMap::~AGameMap()
{
    TilesetTextures.clear();
    LayersVector.clear();
    RenderStatesVector.clear();
    GameMapCollisionLayer.clear();
    LadderCollisionLayer.clear();
    SpawnBaseEnemyPosition.clear();
    SpawnBossEnemyPosition.clear();
}

void AGameMap::InitGameMap()
{
    int ReserveCollisionCapacity = 100;
    GameMapCollisionLayer.reserve(ReserveCollisionCapacity);
    LadderCollisionLayer.reserve(5);

    if (!GameMap.load(ASSETS_PATH + "Map/GarikMap.tmx"))
    {
        std::cerr << "Failed to load map" << std::endl;
        return;
    }

    LoadTilesets(GameMap);
    CheckLayers(GameMap);
    ProcessCollisionLayers(GameMap);
}

void AGameMap::LoadTilesets(const tmx::Map& GameMap)
{
    const auto& Tilesets = GameMap.getTilesets();
    for (const auto& Tileset : Tilesets)
    {
        sf::Texture Texture;
        std::string ImagePath = Tileset.getImagePath();

        if (Texture.loadFromFile(Tileset.getImagePath()))
        {
            TilesetTextures[Tileset.getFirstGID()] = std::move(Texture);
        }
        else
        {
            std::cerr << "Error loading texture: " << ImagePath << std::endl;
        }

        for (const auto& Tile : Tileset.getTiles())
        {
            if (!Tile.animation.frames.empty())
            {
                SAnimatedTile AnimTile;
                AnimTile.TotalDuration = 0.f;

                for (const auto& Frame : Tile.animation.frames)
                {
                    AnimTile.TotalDuration += Frame.duration / 1000.f;
                    AnimTile.FrameDurations.emplace_back(Frame.duration / 1000.f);
                    AnimTile.FrameIDs.emplace_back(Frame.tileID + Tileset.getFirstGID());
                }

                uint32_t TileGlobalID = Tile.ID + Tileset.getFirstGID();
                AnimatedTiles[TileGlobalID] = AnimTile;
                TileAnimator.AnimationTimers[TileGlobalID] = 1.f; // Initialize animation timer for the tile
            }
        }
    }
}

void AGameMap::CheckLayers(const tmx::Map& GameMap)
{
    for (const auto& Layer : GameMap.getLayers())
    {
        if (Layer->getType() == tmx::Layer::Type::Tile)
        {
            const auto* TileLayerPtr = dynamic_cast<const tmx::TileLayer*>(Layer.get());
            if (Layer->getName() == "AnimationTiles")
            {
                ProcessAnimationLayer(TileLayerPtr);
            }
            else
            {
                ProcessTileLayer(TileLayerPtr);
            }
        }
    }
}

void AGameMap::ProcessTileLayer(const tmx::TileLayer* TileLayerPtr)
{
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
                continue;
            }

            uint32_t CurrentFrameID = TileAnimator.GetCurrentFrame(Tile.ID);
            const sf::Texture* CurrentTexturePtr = nullptr;
            uint32_t FirstGID = 0;
            for (const auto& Pair : TilesetTextures)
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

            if (!CurrentTexturePtr)
            {
                continue;
            }

            uint32_t LocalTileID = CurrentFrameID - FirstGID;
            uint32_t tilesPerRow = CurrentTexturePtr->getSize().x / 16;
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

            if (AnimatedTiles.find(Tile.ID) != AnimatedTiles.end())
            {
                AnimatedTiles[Tile.ID].Position = sf::Vector2f(x * 16.f, y * 16.f);
            }
        }
    }

    for (const auto& Pair : LayerVertexMap)
    {
        LayersVector.emplace_back(Pair.second);
        RenderStatesVector.emplace_back();
        RenderStatesVector.back().texture = Pair.first;
    }
}

void AGameMap::ProcessCollisionLayers(const tmx::Map& GameMap)
{
    for (const auto& Layer : GameMap.getLayers())
    {
        if (Layer->getType() == tmx::Layer::Type::Object)
        {
            const auto* ObjectLayer = dynamic_cast<const tmx::ObjectGroup*>(Layer.get());
            if (ObjectLayer && ObjectLayer->getName() == "Obstacles")
            {
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    tmx::FloatRect TmxRect = Object.getAABB();
                    GameMapCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
            }
            else if (ObjectLayer && ObjectLayer->getName() == "DamageCollision")
            {
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    tmx::FloatRect TmxRect = Object.getAABB();
                    DamageCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
            }
            else if (ObjectLayer && ObjectLayer->getName() == "Ladders")
            {
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    tmx::FloatRect TmxRect = Object.getAABB();
                    LadderCollisionLayer.emplace_back(TmxRect.left, TmxRect.top, TmxRect.width, TmxRect.height);
                }
            }
            else if (ObjectLayer && ObjectLayer->getName() == "SpawnEnemy")
            {
                for (const auto& Object : ObjectLayer->getObjects())
                {
                    sf::Vector2f TmxRect = {Object.getPosition().x, Object.getPosition().y};
                    if (Object.getUID() == 399)
                    {
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

void AGameMap::ProcessAnimationLayer(const tmx::TileLayer* AnimationLayerPtr)
{
    const auto& Tiles = AnimationLayerPtr->getTiles();
    const auto& MapSize = AnimationLayerPtr->getSize();

    for (uint32_t y = 0; y < MapSize.y; ++y)
    {
        for (uint32_t x = 0; x < MapSize.x; ++x)
        {
            tmx::TileLayer::Tile Tile = Tiles[y * MapSize.x + x];
            if (Tile.ID == 0)
            {
                continue; // Пропускаем пустые тайлы
            }

            if (AnimatedTiles.find(Tile.ID) != AnimatedTiles.end())
            {
                AnimatedTiles[Tile.ID].Position = sf::Vector2f(x * 16.f, y * 16.f);
                TileAnimator.AnimationTimers[Tile.ID] = 1.f; // Инициализация таймера для анимированного тайла
            }
        }
    }
}

void AGameMap::UpdateAnimatedTiles(float DeltaTime)
{
    TileAnimator.Update(DeltaTime);
}

uint32_t AGameMap::GetCurrentFrame(uint32_t TileID)
{
    return TileAnimator.GetCurrentFrame(TileID);
}

std::vector<sf::FloatRect> AGameMap::GetGameMapCollisionVector() const
{
    return GameMapCollisionLayer;
}

std::vector<sf::FloatRect> AGameMap::GetDamageCollisionVector() const
{
    return DamageCollisionLayer;
}

std::vector<sf::FloatRect> AGameMap::GetLadderCollisionVector() const
{
    return LadderCollisionLayer;
}

void AGameMap::DrawGameMap(sf::RenderWindow& Window)
{
    for (size_t i = 0; i < LayersVector.size(); ++i)
    {
        Window.draw(LayersVector[i], RenderStatesVector[i]);
    }
    TileAnimator.DrawTileAnimator(Window);
}
