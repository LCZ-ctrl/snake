#include "utils.h"
#include "config.h"

// Helper function to check if 'pos' is inside the snake body
bool ElementInDeque(Vector2 element, const std::deque<Vector2>& dq) {
    for (const auto& v : dq) {
        if (Vector2Equals(v, element))
            return true;
    }
    return false;
}

// Helper function to check if 'pos' is inside the obstacles list
bool ElementInVector(Vector2 element, const std::vector<Vector2>& vec) {
    for (const auto& v : vec) {
        if (Vector2Equals(v, element))
            return true;
    }
    return false;
}

bool eventTriggered(double interval) {
    double currentTime = GetTime();
    if (currentTime - lastUpdateTime >= interval) {
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}
