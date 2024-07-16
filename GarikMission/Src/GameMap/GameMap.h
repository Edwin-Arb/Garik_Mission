#pragma once

#include <tmxlite/Layer.hpp>
#include <tmxlite/Map.hpp>

#include "TileAnimator.h"
#include "../Constants.h"
#include "../Player/Player.h"


/**
 * @brief Класс, представляющий игровую карту.
 */
class AGameMap
{
public:
    AGameMap(APlayer& Player); // Конструктор с ссылкой на игрока
    ~AGameMap(); // Деструктор

    void InitGameMap(); // Инициализация игровой карты
    void LoadTilesets(const tmx::Map& GameMap); // Загрузка тайлсетов
    void CheckLayers(const tmx::Map& GameMap); // Проверка слоев карты
    void ProcessTileLayer(const tmx::TileLayer* TileLayerPtr); // Обработка тайлового слоя
    void ProcessAnimationLayer(const tmx::TileLayer* AnimationLayerPtr); // Обработка анимационного слоя
    void ProcessCollisionLayers(const tmx::Map& GameMap); // Обработка слоев коллизий

    void UpdateAnimatedTiles(float DeltaTime); // Обновление анимированных тайлов
    void DrawGameMap(sf::RenderWindow& Window); // Отрисовка игровой карты

    uint32_t GetCurrentFrame(uint32_t TileID); // Получение текущего кадра для тайла

    std::vector<sf::FloatRect> GetGameMapCollisionVector() const; // Получение вектора коллизий игровой карты
    std::vector<sf::FloatRect> GetDamageCollisionVector() const; // Получение вектора коллизий участков карты, которые наносят урон
    std::vector<sf::FloatRect> GetLadderCollisionVector() const; // Получение вектора коллизий лестниц

    std::map<uint32_t, sf::Texture> TilesetTextures; // Текстуры тайлов
    std::unordered_map<uint32_t, SAnimatedTile> AnimatedTiles; // Анимированные тайлы

    std::vector<sf::Vector2f> SpawnBaseEnemyPosition; // Вектор позиций базовых врагов
    std::vector<sf::Vector2f> SpawnBossEnemyPosition; // Вектор позиций босса врагов
private:
    APlayer& PlayerRef; // Ссылка на игрока
    ATileAnimator TileAnimator; // Аниматор тайлов
    tmx::Map GameMap; // Игровая карта
    std::vector<sf::VertexArray> LayersVector; // Вектор вершинных массивов слоев
    std::vector<sf::RenderStates> RenderStatesVector; // Вектор состояний отрисовки
    std::vector<sf::FloatRect> GameMapCollisionLayer; // Вектор коллизий игровой карты
    std::vector<sf::FloatRect> DamageCollisionLayer; // Вектор коллизий участков карты, которые наносят урон
    std::vector<sf::FloatRect> LadderCollisionLayer; // Вектор коллизий лестниц
};
