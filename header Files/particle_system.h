#pragma once
#include <vector>
#include <raylib.h>

struct Particle {
    Vector2 position;
    Vector2 velocity;
    float lifetime;
    float maxLifetime;
    float size;
    Color color;
    bool shrinkOverTime;
};

class ParticleSystem {
public:
    std::vector<Particle> particles;

    void Update(float deltaTime);
    void Draw();
    void SpawnParticles(Vector2 spawnPos, int count,
        float minSpeed, float maxSpeed,
        float minLifetime, float maxLifetime,
        float minSize, float maxSize,
        Color baseColor, bool shrink = false);
};
