#pragma once
#include <deque>
#include <raylib.h>

class Snake {
public:
    // The snake body is a sequence of grid coordinates
    // Initialized with 3 segments
    std::deque<Vector2> body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };

    Vector2 direction = { 1,0 }; // the direction for current move
    Vector2 nextDirection = { 1,0 }; // the buffer for the next input

    // Flag to grow the snake when it eats food
    bool addSegment = false; 

    void Draw(Color snakeColor);
    void Reset(); 
    void UpdateDirection(); 
};
