#pragma once

#include <vector>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>

#include "../Constants.h"
#include "../Player/Player.h"

/**
 * @brief Класс, представляющий игровую карту.
 */
class AGameMap
{
public:
    /**
     * @brief Конструктор класса.
     *
     * @param Player Ссылка на объект персонажа.
     */
    AGameMap(APlayer& Player);

    /**
     * @brief Деструктор класса.
     * Нужен для очищения памяти от Tilesets
     */
    ~AGameMap();

    /**
     * @brief Инициализация игровой карты.
     * Загружает карту, тайлсеты и обрабатывает слои.
     */
    void InitGameMap();

private:
    /**
     * @brief Загружает тайлсеты из карты.
     *
     * @param GameMap Карта игры, содержащая тайлсеты.
     */
    void LoadTilesets(const tmx::Map& GameMap);

    /**
     * @brief Проверяет слои карты и вызывает соответствующие функции для их обработки.
     *
     * @param GameMap Карта игры, содержащая слои.
     */
    void CheckLayers(const tmx::Map& GameMap);

    /**
     * @brief Обрабатывает тайловый слой карты.
     *
     * @param TileLayerPtr Указатель на тайловый слой для обработки.
     */
    void ProcessTileLayer(const tmx::TileLayer* TileLayerPtr);

    /**
     * @brief Обработка коллизий карты.
     *
     * @param GameMap Карта игры, содержащая слои коллизий.
     */
    void ProcessCollisionLayers(const tmx::Map& GameMap);

public:
    /**
     * @brief Возвращает вектор прямоугольников коллизий на карте.
     *
     * @return Вектор прямоугольников коллизий.
     */
    std::vector<sf::FloatRect> GetGameMapCollisionVector() const;

    /**
     * @brief Возвращает вектор прямоугольников лестниц на карте.
     *
     * @return Вектор прямоугольников лестниц.
     */
    std::vector<sf::FloatRect> GetLadderCollisionVector() const;


    /**
     * @brief Отрисовка игровой карты.
     *
     * @param Window Окно для отображения карты.
     */
    void DrawGameMap(sf::RenderWindow& Window) const;

    std::vector<sf::Vector2f> SpawnBaseEnemyPosition;
    std::vector<sf::Vector2f> SpawnBossEnemyPosition;

private:
    APlayer& PlayerRef; // Ссылка на объект персонажа

    tmx::Map GameMap; // Объект карты из tmx

    std::unordered_map<uint32_t, sf::Texture> TilesetTextures; // Карта для хранения текстур тайлсетов

    std::vector<sf::RenderStates> RenderStatesVector; // Сохраняем RenderStates для каждого слоя

    std::vector<sf::VertexArray> LayersVector; // Вектор вершин для отрисовки тайлов

    std::vector<sf::FloatRect> GameMapCollisionLayer; // Вектор прямоугольников коллизий

    std::vector<sf::FloatRect> LadderCollisionLayer; // Вектор прямоугольников лестниц для коллизий
};
