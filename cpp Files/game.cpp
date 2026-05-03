#include "game.h"
#include "config.h"
#include "utils.h"
#include "high_score.h"
#include <cmath>

Game::Game() {
    InitAudioDevice();
    eatSound = LoadSound("Sounds/eat.mp3");
    wallSound = LoadSound("Sounds/wall.mp3");
    eatVolume = 0.3f;
    wallVolume = 0.3f;
    SetSoundVolume(eatSound, eatVolume);
    SetSoundVolume(wallSound, wallVolume);
    musicVolume1 = 0.4f;
    musicVolume2 = 0.6f;
    musicVolume3 = 0.4f;
    musicVolume4 = 0.2f;
    musicVolume5 = 0.5f;
    musicVolume6 = 0.4f;
    musicVolume7 = 0.3f;
    musicVolume8 = 0.5f;
    Music music1 = LoadMusicStream("Sounds/a brief love affair.mp3");
    Music music2 = LoadMusicStream("Sounds/winter heartbeats.mp3");
    Music music3 = LoadMusicStream("Sounds/quite rooms.mp3");
    Music music4 = LoadMusicStream("Sounds/la vie en rose.mp3");
    Music music5 = LoadMusicStream("Sounds/Lovers.mp3");
    Music music6 = LoadMusicStream("Sounds/be there.mp3");
    Music music7 = LoadMusicStream("Sounds/Strawberry Kisses.mp3");
    Music music8 = LoadMusicStream("Sounds/love galore.mp3");
    music1.looping = false;
    music2.looping = false;
    music3.looping = false;
    music4.looping = false;
    music5.looping = false;
    music6.looping = false;
    music7.looping = false;
    music8.looping = false;
    SetMusicVolume(music1, musicVolume1);
    SetMusicVolume(music2, musicVolume2);
    SetMusicVolume(music3, musicVolume3);
    SetMusicVolume(music4, musicVolume4);
    SetMusicVolume(music5, musicVolume5);
    SetMusicVolume(music6, musicVolume6);
    SetMusicVolume(music7, musicVolume7);
    SetMusicVolume(music8, musicVolume8);
    playlist.push_back(music1);
    playlist.push_back(music2);
    playlist.push_back(music3);
    playlist.push_back(music4);
    playlist.push_back(music5);
    playlist.push_back(music6);
    playlist.push_back(music7);
    playlist.push_back(music8);
    currentMusicIndex = 0;
    musicPaused = false;
    musicDurations.push_back(209.0f);
    musicDurations.push_back(136.0f);
    musicDurations.push_back(151.0f);
    musicDurations.push_back(182.0f);
    musicDurations.push_back(94.0f);
    musicDurations.push_back(182.0f);
    musicDurations.push_back(80.0f);
    musicDurations.push_back(202.0f);
    PlayMusicStream(playlist[currentMusicIndex]);

    obstacleTexture = LoadTexture("Graphics/bomb.png");
}

Game::~Game() {
    UnloadSound(eatSound);
    UnloadSound(wallSound);
    for (unsigned int i = 0; i < playlist.size(); i++) {
        UnloadMusicStream(playlist[i]);
    }
    CloseAudioDevice();

    UnloadTexture(obstacleTexture);
}

void Game::GenerateObstacles() {
    obstacles.clear();

    std::vector<Vector2> safetyZone = {
        Vector2{7, 9},
        Vector2{8, 9},
        Vector2{9, 9},
        Vector2{10, 9},
        Vector2{11, 9}
    };

    while (obstacles.size() < 10) {
        Vector2 pos = GenerateRandomCellForObstacles();
        // Obstacles must not spawn on:
        // 1. The snake's body
        // 2. The food's position
        // 3. Another existing obstacle
        if (!ElementInDeque(pos, snake.body) && 
            !Vector2Equals(pos, food.position) && 
            !ElementInVector(pos, obstacles) &&
            !ElementInVector(pos, safetyZone)) { 
            obstacles.push_back(pos);
        }
    }
}

Vector2 Game::GenerateRandomCellForObstacles() {
    float x = GetRandomValue(0, cellCount - 1);
    float y = GetRandomValue(0, cellCount - 1);
    return Vector2{ x, y };
}

void Game::StartScreenShake(float duration, float magnitude) {
    screenShakeDuration = duration;
    screenShakeTimer = duration;
    screenShakeMagnitude = magnitude;
}

Vector2 Game::GetScreenShakeOffset() {
    if (screenShakeTimer > 0.0f) {
        float progress = (screenShakeDuration - screenShakeTimer) / screenShakeDuration;
        float currentMag = (1.0f - progress) * screenShakeMagnitude;
        float offsetX = GetRandomValue((int)(-currentMag), (int)(currentMag));
        float offsetY = GetRandomValue((int)(-currentMag), (int)(currentMag));
        return { offsetX, offsetY };
    }
    return { 0, 0 };
}

void Game::UpdateScreenShake(float deltaTime) {
    if (screenShakeTimer > 0.0f) {
        screenShakeTimer -= deltaTime;
        if (screenShakeTimer < 0.0f) {
            screenShakeTimer = 0.0f;
        }
    }
}

void Game::SpawnCollisionParticles() {
    Vector2 collisionPos = {
        offset + snake.body[0].x * cellSize + cellSize / 2.0f,
        offset + snake.body[0].y * cellSize + cellSize / 2.0f
    };
    particleSystem.SpawnParticles(
        collisionPos,
        50,
        0.0f, 100.0f,
        0.3f, 0.3f,
        3.0f, 6.0f,
        Color{ 255, 0, 0, 200 },
        false
    );
}

void Game::Draw() {
    UpdateScreenShake(GetFrameTime());
    Vector2 shakeOffset = GetScreenShakeOffset();
    BeginMode2D(Camera2D{ {shakeOffset.x, shakeOffset.y}, {0, 0}, 0.0f, 1.0f });
    food.Draw();

    Color currentSnakeColor;
    if (mode == 2) {
        currentSnakeColor = BLACK;
    }
    else {
        currentSnakeColor = darkGreen;
    }
    snake.Draw(currentSnakeColor);

    if (mode == 2) {
        for (auto& obs : obstacles) {
            Vector2 position = {
                (float)offset + obs.x * cellSize,
                (float)offset + obs.y * cellSize
            };
            float scale = (float)cellSize / (float)obstacleTexture.width;
            DrawTextureEx(obstacleTexture, position, 0.0f, scale, WHITE);
        }
    }
    if (!running || paused) {
        DrawRectangleRounded(
            Rectangle{
                (float)offset - 5,
                (float)offset - 5,
                (float)cellSize * cellCount + 10,
                (float)cellSize * cellCount + 10
            },
            0.1f,
            10,
            Color{ 0, 0, 0, 230 }
        );
    }
    if (!running) {
        const char* gameOverText = "Game Over!";
        Vector2 center = {
            offset + cellSize * cellCount / 2.0f,
            offset + cellSize * cellCount / 2.0f
        };
        DrawText(
            gameOverText,
            center.x - MeasureText(gameOverText, 60) / 2,
            center.y - 60,
            60,
            Color{ 230, 41, 55, 255 }
        );
        if (newRecordAchieved) {
            const char* newRecordText = "New Record!";
            DrawText(
                newRecordText,
                center.x - MeasureText(newRecordText, 30) / 2 - 7,
                center.y - 100,
                30,
                Color{ 255, 215, 0, 255 }
            );
        }
        const char* promptText = "Press [M] to Return to Menu";
        DrawText(
            promptText,
            center.x - MeasureText(promptText, 30) / 2,
            center.y + 20,
            30,
            RED
        );
    }
    else if (paused) {
        const char* pausedText = "Game Paused!";
        const char* resumeText = "Press [Enter] to Resume";
        const char* menuText = "Press [M] to Menu";
        const char* exitText = "Press [E] to Exit";
        Vector2 center = {
            offset + cellSize * cellCount / 2.0f,
            offset + cellSize * cellCount / 2.0f
        };
        DrawText(
            pausedText,
            center.x - MeasureText(pausedText, 60) / 2,
            center.y - 80,
            60,
            Color{ 253, 249, 0, 255 }
        );
        int yOffset = 40;
        DrawText(
            resumeText,
            center.x - MeasureText(resumeText, 30) / 2,
            center.y + yOffset,
            30,
            YELLOW
        );
        DrawText(
            menuText,
            center.x - MeasureText(menuText, 30) / 2,
            center.y + yOffset + 40,
            30,
            YELLOW
        );
        DrawText(
            exitText,
            center.x - MeasureText(exitText, 30) / 2,
            center.y + yOffset + 80,
            30,
            YELLOW
        );
    }
    particleSystem.Draw();
    EndMode2D();
}

void Game::Update(float deltaTime) {
    if (!running) return;
    snake.UpdateDirection(); // apply player input
    Vector2 nextHead = Vector2Add(snake.body[0], snake.direction);

    // Collision Check 1: Walls
    if (nextHead.x >= cellCount || nextHead.x < 0 || nextHead.y >= cellCount || nextHead.y < 0) {
        GameOver();
        SpawnCollisionParticles();
        return;
    }

    // Collision Check 2: Snake's own body
    std::deque<Vector2> collisionBody = snake.body;
    if (!snake.addSegment) {
        collisionBody.pop_back();
    }
    for (unsigned int i = 0; i < collisionBody.size(); i++) {
        if (Vector2Equals(nextHead, collisionBody[i])) {
            GameOver();
            SpawnCollisionParticles();
            return;
        }
    }

    // Hard mode collision check
    if (mode == 2) {
        for (unsigned int i = 0; i < obstacles.size(); i++) {
            if (Vector2Equals(nextHead, obstacles[i])) {
                GameOver();
                SpawnCollisionParticles();
                return;
            }
        }
    }

    bool foodEaten = Vector2Equals(nextHead, food.position);
    snake.body.push_front(nextHead);
    if (foodEaten) {
        score++;
        PlaySound(eatSound);
        Vector2 spawnPos = {
            offset + food.position.x * cellSize + cellSize / 2.0f,
            offset + food.position.y * cellSize + cellSize / 2.0f
        };
        if (mode == 2) {
            particleSystem.SpawnParticles(
                spawnPos,
                35,
                0.0f, 50.0f,
                0.5f, 1.5f,
                3.0f, 8.0f,
                Color{ 255, 0, 255, 255 },
                false
            );
            particleSystem.SpawnParticles(
                spawnPos,
                15,
                0.0f, 50.0f,
                0.5f, 1.5f,
                3.0f, 8.0f,
                Color{ 0, 255, 255, 255 },
                false
            );
        }
        else {
            particleSystem.SpawnParticles(
                spawnPos,
                35,
                0.0f, 50.0f,
                0.5f, 1.5f,
                3.0f, 8.0f,
                Color{ 255, 215, 0, 255 },
                false
            );
            particleSystem.SpawnParticles(
                spawnPos,
                15,
                0.0f, 50.0f,
                0.5f, 1.5f,
                3.0f, 8.0f,
                Color{ 173, 204, 96, 255 },
                false
            );
        }
        if (mode == 2)
            food.position = food.GenerateRandomPos(snake.body, obstacles);
        else
            food.position = food.GenerateRandomPos(snake.body);
    }
    else {
        if (!snake.addSegment)
            snake.body.pop_back();
        else
            snake.addSegment = false;
    }
}

void Game::SpawnAtomicBombParticles() {
    Vector2 explosionPos = {
        offset + snake.body[0].x * cellSize + cellSize / 2.0f,
        offset + snake.body[0].y * cellSize + cellSize / 2.0f
    };
    particleSystem.SpawnParticles(explosionPos, 100, 0.0f, 200.0f, 0.5f, 1.0f, 5.0f, 10.0f, Color{ 255, 165, 0, 255 }, false);
    particleSystem.SpawnParticles(explosionPos, 50, 0.0f, 200.0f, 0.5f, 1.0f, 5.0f, 10.0f, Color{ 255, 255, 0, 255 }, false);
    particleSystem.SpawnParticles(explosionPos, 50, 0.0f, 200.0f, 0.5f, 1.0f, 5.0f, 10.0f, Color{ 0, 0, 0, 255 }, false);
    particleSystem.SpawnParticles(explosionPos, 50, 0.0f, 200.0f, 0.5f, 1.0f, 5.0f, 10.0f, Color{ 255, 255, 255, 255 }, false);
}

void Game::GameOver() {
    int currentScore = score;
    int savedHighScore = GetHighScoreForMode(mode);
    if (currentScore > savedHighScore) {
        SetHighScoreForMode(mode, currentScore);
        newRecordAchieved = true;
    }
    else {
        newRecordAchieved = false;
    }
    finalScore = score;
    running = false;
    paused = false;
    PlaySound(wallSound);
    StartScreenShake(0.6f, 10.0f);
    if (mode == 2) {
        SpawnAtomicBombParticles();
    }
}

void Game::Pause() {
    paused = !paused;
}

void Game::ReturnToMenu() {
    menuActive = true;
    running = false;
    paused = false;
    snake.Reset();
    score = 0;
    finalScore = 0;
    obstacles.clear();
}
