#ifndef GAMEMAP_HPP
#define GAMEMAP_HPP

#include <vector>
#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include <tmxlite/Tileset.hpp>

#include "../Constants.h"
#include "../Player/Player.h"


/**
 * Класс, представляющий игровую карту.
 */
class AGameMap
{
public:
    /**
     * Конструктор класса.
     *
     * @param Player Ссылка на объект игрока.
     */
    AGameMap(APlayer& Player);

    /**
     * Деструктор класса.
     * Нужен для очищения памяти от Tilesets
     */
    ~AGameMap();

    /**
     * Инициализация игровой карты.
     * Загружает карту, тайлсеты и обрабатывает слои.
     */
    void InitGameMap();

private:
    /**
     * Загружает тайлсеты из карты.
     *
     * @param GameMap Карта игры, содержащая тайлсеты.
     */
    void LoadTilesets(const tmx::Map& GameMap);

    /**
     * Проверяет слои карты и вызывает соответствующие функции для их обработки.
     *
     * @param GameMap Карта игры, содержащая слои.
     */
    void CheckLayers(const tmx::Map& GameMap);

    /**
     * Обрабатывает тайловый слой карты.
     *
     * @param TileLayerPtr Указатель на тайловый слой для обработки.
     */
    void ProcessTileLayer(const tmx::TileLayer* TileLayerPtr);

    /**
     * Обработка коллизий карты.
     *
     * @param GameMap Карта игры, содержащая слои коллизий.
     */
    void ProcessCollisionLayers(const tmx::Map& GameMap);

public:
    /**
     * Возвращает вектор прямоугольников коллизий на карте.
     *
     * @return Вектор прямоугольников коллизий.
     */
    std::vector<sf::FloatRect> GetGameMapCollisionVector() const;

    /**
     * Возвращает вектор прямоугольников лестниц на карте.
     *
     * @return Вектор прямоугольников лестниц.
     */
    std::vector<sf::FloatRect> GetLadderCollisionVector() const;


    /**
     * Отрисовка игровой карты.
     *
     * @param Window Окно для отображения карты.
     * @param View
     */
    void DrawGameMap(sf::RenderWindow& Window, const sf::View& View) const;

private:

    APlayer& PlayerRef; // Ссылка на объект игрока

    tmx::Map GameMap; // Объект карты из tmx

    std::unordered_map<uint32_t, sf::Texture> TilesetTextures; // Карта для хранения текстур тайлсетов

    std::vector<sf::RenderStates> RenderStatesVector; // Сохраняем RenderStates для каждого слоя

    std::vector<sf::VertexArray> LayersVector; // Вектор вершин для отрисовки тайлов

    std::vector<sf::FloatRect> GameMapCollisionLayer; // Вектор прямоугольников коллизий

    std::vector<sf::FloatRect> LadderCollisionLayer; // Вектор прямоугольников лестниц для коллизий
};

#endif GAMEMAP_HPP
