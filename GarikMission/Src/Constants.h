#pragma once
#include <cassert>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;

constexpr int BULLET_DAMAGE = 20;
constexpr int DEATH = 0;

constexpr float PAWN_SPEED = 300.f;
constexpr float BULLET_SPEED = 500.f;
constexpr float GRAVITY = -2.f;
constexpr float ZOOM_FACTOR = 1.2f;

const sf::Vector2f PLAYER_SIZE = {11.f, 16.f};
const sf::Vector2f ENEMY_SIZE = {10.f, 15.f};
const sf::Vector2f BULLET_SIZE = {3.f, 3.f};
const sf::Vector2f DRAW_SCALE = {8.f, 8.f};
const sf::Vector2f BULLET_DIRECTION = {400.f, 0.f};

const std::string ASSETS_PATH = "Assets/";

