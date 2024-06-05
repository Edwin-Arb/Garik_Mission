#pragma once
#include <vector>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include "../Constants.h"
#include "../Player/Player.h"


class AGameMap
{
public:
    AGameMap(APlayer& Player);

    ~AGameMap() = default;

    void InitGameMap();
    void DrawGameMap(sf::RenderWindow& Window, float DrawScale) const;

    // Возвращает вектор коллизий
    std::vector<sf::FloatRect> GetCollisionVector() const;

    // Проверяет коллизию игрока с коллизией карты
    bool CheckCollision(const sf::FloatRect& PlayerBounds) const;

private:
    // Получает текстурные координаты тайла
    sf::IntRect GetTileTextureRect(const tmx::Tileset& Tileset, const std::uint32_t& TileID) const;

    // Отрисовывает слой тайлов
    void DrawLayer(const tmx::TileLayer& Layer, sf::RenderTarget& target, float DrawScale);

private:
    // Ссылка на объект игрока
    APlayer& PlayerRef;
    
    // Объект карты из tmx
    tmx::Map GameMap;

    // Спрайт карты
    sf::Sprite MapSprite;

    // Текстура карты
    sf::RenderTexture MapTexture;

    // Массив вершин для отрисовки тайлов
    sf::VertexArray Vertices;

    // Вектор текстур тайлсетов
    std::vector<sf::Texture> TilesetTextures;

    // Вектор препятствий
    std::vector<sf::FloatRect> GetCollisionLayer;
};
