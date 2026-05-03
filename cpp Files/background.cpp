#include "background.h"
#include "config.h"
#include <cmath>

void DrawGradientBackground() {
    static float time = 0.0f;
    time += GetFrameTime();
    Color topColor = {
        (unsigned char)(127 + 128 * sin(time)),
        (unsigned char)(127 + 128 * sin(time + 2.0f)),
        (unsigned char)(127 + 128 * sin(time + 4.0f)),
        255
    };
    Color bottomColor = {
        (unsigned char)(127 + 128 * cos(time)),
        (unsigned char)(127 + 128 * cos(time + 2.0f)),
        (unsigned char)(127 + 128 * cos(time + 4.0f)),
        255
    };
    DrawRectangleGradientV(0, 0, GetScreenWidth(), GetScreenHeight(), topColor, bottomColor);
}
