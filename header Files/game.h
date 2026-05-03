#pragma once
#include "snake.h"
#include "food.h"
#include "particle_system.h"
#include <vector>
#include <raylib.h>
#include <raymath.h>

class Game {
public:
    Snake snake = Snake();
    Food food = Food(snake.body);
    ParticleSystem particleSystem;

    bool running = false;
    bool paused = false;
    bool menuActive = true;
    bool modeSelectionActive = false;
    bool showInstructions = false;
    bool showAcknowledgement = false;
    int score = 0;
    int finalScore = 0;
    bool newRecordAchieved = false;
    Sound eatSound;
    Sound wallSound;
    int mode = 1;
    double moveInterval = 0.2; // time interval between moves, to adjust speed
    std::vector<Vector2> obstacles;
    std::vector<Music> playlist;
    int currentMusicIndex;
    bool musicPaused;

    float musicVolume1, musicVolume2, musicVolume3, musicVolume4, 
        musicVolume5, musicVolume6, musicVolume7, musicVolume8;
    std::vector<float> musicDurations;
    float eatVolume, wallVolume;

    float screenShakeTimer = 0.0f;
    float screenShakeDuration = 0.0f;
    float screenShakeMagnitude = 0.0f;

    Texture2D obstacleTexture;

    Game();
    ~Game();

    void GenerateObstacles();
    Vector2 GenerateRandomCellForObstacles();
    void StartScreenShake(float duration, float magnitude);
    Vector2 GetScreenShakeOffset();
    void UpdateScreenShake(float deltaTime);
    void SpawnCollisionParticles();
    void Draw();
    void Update(float deltaTime);
    void SpawnAtomicBombParticles();
    void GameOver();
    void Pause();
    void ReturnToMenu();
};
