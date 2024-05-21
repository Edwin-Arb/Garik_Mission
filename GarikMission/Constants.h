﻿#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

struct Vector2D
{
    float X = 0.f;
    float Y = 0.f;
};

typedef Vector2D Position2D;

struct Rectangle
{
    Position2D position2D;
    Vector2D size;
};

// Move Pawn
enum class EPawnDirection
{
    EPD_Up,
    EPD_Left, 
    EPD_Down, 
    EPD_Right,
    EPD_Jump,
    
    EPD_StartPosition
};

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 720;
constexpr float SPEED_PAWN = 300.f;
constexpr float GRAVITY = -6.f;
constexpr float MAP_SURFACE = SCREEN_HEIGHT - 200.f;

const sf::Vector2f PLAYER_SIZE = {11.f, 16.f};
const sf::Vector2f ENEMY_SIZE = {10.f, 15.f};
const sf::Vector2f DRAW_SCALE = {8.f, 8.f};

const std::string RESOURCES_PATH = "Resources/";

