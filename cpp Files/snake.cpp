#include "snake.h"
#include "config.h"

// Render the snake
void Snake::Draw(Color snakeColor) {
    for (unsigned int i = 0; i < body.size(); i++) {
        float x = body[i].x;
        float y = body[i].y;
        Rectangle segment = { offset + x * cellSize, offset + y * cellSize, (float)cellSize, (float)cellSize };

        DrawRectangleRounded(segment, 0.4, 6, snakeColor);
    }
}

// Re-initialize body and direction to default values
void Snake::Reset() {
    body = { Vector2{6,9}, Vector2{5,9}, Vector2{4,9} };
    direction = { 1,0 };
    nextDirection = { 1,0 };
}

// Only update the direction if the nextDirection is NOT the exact opposite
void Snake::UpdateDirection() {
    if ((nextDirection.x + direction.x != 0) || (nextDirection.y + direction.y != 0)) {
        direction = nextDirection;
    }
}
