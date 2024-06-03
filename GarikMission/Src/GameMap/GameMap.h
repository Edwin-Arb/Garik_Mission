#pragma once
#include "../Constants.h"
#include "../Player/Player.h"

#include <tmxlite/Map.hpp>
#include <tmxlite/Layer.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>
#include <unordered_map>
#include <vector>


class AGameMap
{
public:
    AGameMap() = delete;
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
    tmx::Map GameMap; // Объект карты из tmx
    sf::Sprite MapSprite; // Спрайт карты
    sf::RenderTexture MapTexture; // Текстура карты

    sf::VertexArray Vertices; // Массив вершин для отрисовки тайлов

    std::vector<sf::Texture> TilesetTextures; // Вектор текстур тайлсетов
    std::vector<sf::FloatRect> GetCollisionLayer; // Вектор препятствий

    APlayer& PlayerRef; // Ссылка на объект игрока
};
