#pragma once
#include <iostream>
#include <deque>
#include <vector>
#include <raylib.h>

class Food {
public:
    Vector2 position;
    Texture2D texture;
    bool isHardMode = false;

    Food(std::deque<Vector2> snakeBody);
    ~Food();

    void Draw(); // draw food
    Vector2 GenerateRandomCell(); // select a coordinate randomly

    // Generate a food position that does not overlap with the snake or obstacles
    Vector2 GenerateRandomPos(std::deque<Vector2> snakeBody, const std::vector<Vector2>& obstacles = {});
};
