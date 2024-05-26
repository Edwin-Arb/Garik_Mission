#pragma once
#include <cassert>
#include <string>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>


constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr float PAWN_SPEED = 300.f;
constexpr float BULLET_SPEED = 1000.f;
constexpr float GRAVITY = -15.f;
constexpr float MAP_SURFACE = SCREEN_HEIGHT - 100.f;

const sf::Vector2f PLAYER_SIZE = {11.f, 16.f};
const sf::Vector2f ENEMY_SIZE = {10.f, 15.f};
const sf::Vector2f BULLET_SIZE = {2.f, 2.f};
const sf::Vector2f DRAW_SCALE = {8.f, 8.f};
const sf::Vector2f BULLET_DIRECTION = {400.f, 0.f};

const std::string RESOURCES_PATH = "Assets/";

