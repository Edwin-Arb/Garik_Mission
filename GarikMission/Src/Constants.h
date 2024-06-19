#pragma once
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


/**
 * @brief Ширина экрана игры в пикселях.
 */
constexpr int SCREEN_WIDTH = 1280;

/**
 * @brief Высота экрана игры в пикселях.
 */
constexpr int SCREEN_HEIGHT = 720;

/**
 * @brief Урон, который наносит пуля персонажам (пешкам).
 */
constexpr int BULLET_DAMAGE = 20;

/**
 * @brief Значение здоровья, при котором игрок считается мертвым.
 */
constexpr int DEATH = 0;

/**
 * @brief Скорость движения игровых персонажей (пешек).
 */
constexpr float PAWN_SPEED = 300.f;

/**
 * @brief Скорость движения пуль в игре.
 */
constexpr float BULLET_SPEED = 500.f;

/**
 * @brief Значение гравитации, влияющее на падение объектов в игре.
 */
constexpr float GRAVITY = -10.f;

/**
 * @brief Коэффициент масштабирования для различных элементов игры.
 */
constexpr float ZOOM_FACTOR = 1.2f;

/**
 * @brief Размер игрового персонажа (игрока).
 */
const sf::Vector2f PLAYER_SIZE = {12.f, 16.f};

/**
 * @brief Размер игрового персонажа (врага).
 */
const sf::Vector2f ENEMY_SIZE = {10.f, 15.f};

/**
 * @brief Размер пули в игре.
 */
const sf::Vector2f BULLET_SIZE = {3.f, 3.f};

/**
 * @brief Масштаб для отрисовки игровых объектов.
 */
const sf::Vector2f DRAW_SCALE = {8.f, 8.f};

/**
 * @brief Направление по умолчанию для движения пули.
 */
const sf::Vector2f BULLET_DIRECTION = {400.f, 0.f};

/**
 * @brief Путь к папке с ресурсами игры.
 */
const std::string ASSETS_PATH = "Assets/";