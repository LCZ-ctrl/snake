#pragma once
#include <deque>
#include <vector>
#include <raylib.h>
#include <raymath.h>

bool ElementInDeque(Vector2 element, const std::deque<Vector2>& dq); // for snake body
bool ElementInVector(Vector2 element, const std::vector<Vector2>& vec); // for obstacles
bool eventTriggered(double interval); 
