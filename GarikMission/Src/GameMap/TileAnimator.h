#pragma once
#include "../Constants.h"
#include "unordered_map"

// Предварительное объявление
class AGameMap;

struct SAnimatedTile
{
    float TotalDuration = 0.f;
    float ElapsedTime = 0.f;
    size_t CurrentFrame = 0;
    std::vector<float> FrameDurations;
    std::vector<uint32_t> FrameIDs;
    sf::Vector2f Position;
};

class ATileAnimator
{
public:
    explicit ATileAnimator(AGameMap* GameMapPtr);
    ~ATileAnimator();

    void Update(float DeltaTime);
    void DrawTileAnimator(sf::RenderWindow& Window) const;
    uint32_t GetCurrentFrame(uint32_t TileID) const;

    std::unordered_map<uint32_t, float> AnimationTimers;

private:
    AGameMap* GameMapPtr;
};

// /**
//  * @brief Структура для создания анимаций тайлов
//  */
// struct SAnimatedTile
// {
//     float TotalDuration = 0.f; // Общая длительность анимации
//     float ElapsedTime = 0.f;   // Время, прошедшее с начала анимации
//     size_t CurrentFrame = 0;   // Текущий кадр
//
//     std::vector<float> FrameDurations; // Вектор длительностей кадров
//     std::vector<uint32_t> FrameIDs;    // Вектор ID кадров
//
//     sf::Vector2f Position; // Позиция тайла на карте
// };
//
// class ATileAnimator
// {
// public:
//     ATileAnimator() = delete; // Удаляем конструктор по умолчанию
//     ATileAnimator(AGameMap* MapPtr); // Конструктор с указателем на игровую карту
//
//     ~ATileAnimator() = default; // Деструктор по умолчанию
//
//     void Update(float DeltaTime);                   // Обновление анимации
//     void DrawTileAnimator(sf::RenderWindow& Window); // Отрисовка анимированных тайлов
//
//     uint32_t GetCurrentFrame(uint32_t TileID); // Получение текущего кадра для тайла
//
//     std::unordered_map<uint32_t, float> AnimationTimers; // Таймеры для анимации тайлов
//
// private:
//     AGameMap* GameMapPtr; // Указатель на игровую карту
// };
