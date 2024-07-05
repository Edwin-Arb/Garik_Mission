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
constexpr int BULLET_DAMAGE = 10;

/**
 * @brief Значение здоровья, при котором игрок считается мертвым.
 */
constexpr int DEATH = 0;

/**
 * @brief Значение здоровья врагов.
 */
constexpr int ENEMY_MAX_HEALTH = 100;

/**
 * @brief Скорость движения персонажа.
 */
constexpr float PLAYER_SPEED = 100.f;

/**
 * @brief Скорость движения врага.
 */
constexpr float ENEMY_SPEED = 50.f;

/**
 * @brief Скорость движения пуль в игре.
 */
constexpr float BULLET_SPEED = 100.f;

/**
 * @brief Значение гравитации, влияющее на падение объектов в игре.
 */
constexpr float GRAVITY = -5.f;

/**
 * @brief Коэффициент масштабирования для различных элементов игры.
 */
constexpr float ZOOM_FACTOR = 0.2f;

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
const sf::Vector2f DRAW_SCALE = {1.f, 1.f};

/**
 * @brief Направление по умолчанию для движения пули.
 */
const sf::Vector2f BULLET_DIRECTION = {400.f, 0.f};

/**
 * @brief Путь к папке с ресурсами игры.
 */
const std::string ASSETS_PATH = "Assets/";
