#include <iostream>
#include <fstream>
#include <string>
#include <direct.h>
#include <raylib.h>
#include <raymath.h>
#include <deque>
#include <vector>
#include <imgui.h>
#include <rlImGui.h>
#include <cmath>
#include <thread>
#include <atomic>
#include <mutex>
#include <chrono>

using namespace std;

#include "config.h"
#include "high_score.h"
#include "utils.h"
#include "game.h"
#include "background.h"

static std::atomic<bool> g_audioThreadRunning(false);
static std::atomic<bool> g_musicStarted(false);
static std::thread g_audioThread;
static std::mutex g_audioMutex;

int main() {
	CreateHighScoreFolder();

	InitWindow(2 * offset + cellSize * cellCount, 2 * offset + cellSize * cellCount, "Snake!");
	Image icon = LoadImage("Graphics/logo.png");
	SetWindowIcon(icon);
	UnloadImage(icon);

	SetExitKey(0);
	SetTargetFPS(60);
	rlImGuiSetup(true);

	Game game;
	bool shouldExit = false;
	float lastFrameTime = GetTime();

	g_audioThreadRunning = true;
	g_audioThread = std::thread([&game]() {
		using namespace std::chrono;
		while (g_audioThreadRunning.load()) {
			{
				std::lock_guard<std::mutex> lk(g_audioMutex);
				if (g_musicStarted.load() && !game.playlist.empty() && !game.musicPaused) {
					UpdateMusicStream(game.playlist[game.currentMusicIndex]);

					if (GetMusicTimePlayed(game.playlist[game.currentMusicIndex]) >=
						game.musicDurations[game.currentMusicIndex] - 0.1f) {
						StopMusicStream(game.playlist[game.currentMusicIndex]);
						game.currentMusicIndex = (game.currentMusicIndex + 1) % game.playlist.size();
						PlayMusicStream(game.playlist[game.currentMusicIndex]);
						if (game.currentMusicIndex == 0) SetMusicVolume(game.playlist[0], game.musicVolume1);
						else if (game.currentMusicIndex == 1) SetMusicVolume(game.playlist[1], game.musicVolume2);
						else if (game.currentMusicIndex == 2) SetMusicVolume(game.playlist[2], game.musicVolume3);
						else if (game.currentMusicIndex == 3) SetMusicVolume(game.playlist[3], game.musicVolume4);
						else if (game.currentMusicIndex == 4) SetMusicVolume(game.playlist[4], game.musicVolume5);
						else if (game.currentMusicIndex == 5) SetMusicVolume(game.playlist[5], game.musicVolume6);
						else if (game.currentMusicIndex == 6) SetMusicVolume(game.playlist[6], game.musicVolume7);
						else if (game.currentMusicIndex == 7) SetMusicVolume(game.playlist[7], game.musicVolume8);
					}
				}
			}
			std::this_thread::sleep_for(milliseconds(10));
		}
		});

	while (!WindowShouldClose() && !shouldExit) {
		float currentFrameTime = GetTime();
		float deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		if (game.menuActive && !g_musicStarted.load()) {
			std::lock_guard<std::mutex> lk(g_audioMutex);
			if (!game.playlist.empty()) {
				PlayMusicStream(game.playlist[game.currentMusicIndex]);
				if (game.currentMusicIndex == 0) SetMusicVolume(game.playlist[0], game.musicVolume1);
				else if (game.currentMusicIndex == 1) SetMusicVolume(game.playlist[1], game.musicVolume2);
				else if (game.currentMusicIndex == 2) SetMusicVolume(game.playlist[2], game.musicVolume3);
				else if (game.currentMusicIndex == 3) SetMusicVolume(game.playlist[3], game.musicVolume4);
				else if (game.currentMusicIndex == 4) SetMusicVolume(game.playlist[4], game.musicVolume5);
				else if (game.currentMusicIndex == 5) SetMusicVolume(game.playlist[5], game.musicVolume6);
				else if (game.currentMusicIndex == 6) SetMusicVolume(game.playlist[6], game.musicVolume7);
				else if (game.currentMusicIndex == 7) SetMusicVolume(game.playlist[7], game.musicVolume8);
				g_musicStarted.store(true);
				game.musicPaused = false;
			}
		}

		if (IsKeyPressed(KEY_SPACE)) {
			std::lock_guard<std::mutex> lk(g_audioMutex);
			if (!g_musicStarted.load()) {
				if (!game.playlist.empty()) {
					PlayMusicStream(game.playlist[game.currentMusicIndex]);
					if (game.currentMusicIndex == 0) SetMusicVolume(game.playlist[0], game.musicVolume1);
					else if (game.currentMusicIndex == 1) SetMusicVolume(game.playlist[1], game.musicVolume2);
					else if (game.currentMusicIndex == 2) SetMusicVolume(game.playlist[2], game.musicVolume3);
					else if (game.currentMusicIndex == 3) SetMusicVolume(game.playlist[3], game.musicVolume4);
					else if (game.currentMusicIndex == 4) SetMusicVolume(game.playlist[4], game.musicVolume5);
					else if (game.currentMusicIndex == 5) SetMusicVolume(game.playlist[5], game.musicVolume6);
					else if (game.currentMusicIndex == 6) SetMusicVolume(game.playlist[6], game.musicVolume7);
					else if (game.currentMusicIndex == 7) SetMusicVolume(game.playlist[7], game.musicVolume8);
					g_musicStarted.store(true);
					game.musicPaused = false;
				}
			}
			else {
				if (!game.musicPaused) {
					PauseMusicStream(game.playlist[game.currentMusicIndex]);
					game.musicPaused = true;
				}
				else {
					ResumeMusicStream(game.playlist[game.currentMusicIndex]);
					game.musicPaused = false;
				}
			}
		}

		if (IsKeyPressed(KEY_A)) {
			std::lock_guard<std::mutex> lk(g_audioMutex);
			if (g_musicStarted.load() && !game.playlist.empty()) {
				StopMusicStream(game.playlist[game.currentMusicIndex]);
				game.currentMusicIndex = (game.currentMusicIndex - 1 + game.playlist.size()) % game.playlist.size();
				PlayMusicStream(game.playlist[game.currentMusicIndex]);
				if (game.currentMusicIndex == 0) SetMusicVolume(game.playlist[0], game.musicVolume1);
				else if (game.currentMusicIndex == 1) SetMusicVolume(game.playlist[1], game.musicVolume2);
				else if (game.currentMusicIndex == 2) SetMusicVolume(game.playlist[2], game.musicVolume3);
				else if (game.currentMusicIndex == 3) SetMusicVolume(game.playlist[3], game.musicVolume4);
				else if (game.currentMusicIndex == 4) SetMusicVolume(game.playlist[4], game.musicVolume5);
				else if (game.currentMusicIndex == 5) SetMusicVolume(game.playlist[5], game.musicVolume6);
				else if (game.currentMusicIndex == 6) SetMusicVolume(game.playlist[6], game.musicVolume7);
				else if (game.currentMusicIndex == 7) SetMusicVolume(game.playlist[7], game.musicVolume8);
			}
		}

		if (IsKeyPressed(KEY_D)) {
			std::lock_guard<std::mutex> lk(g_audioMutex);
			if (g_musicStarted.load() && !game.playlist.empty()) {
				StopMusicStream(game.playlist[game.currentMusicIndex]);
				game.currentMusicIndex = (game.currentMusicIndex + 1) % game.playlist.size();
				PlayMusicStream(game.playlist[game.currentMusicIndex]);
				if (game.currentMusicIndex == 0) SetMusicVolume(game.playlist[0], game.musicVolume1);
				else if (game.currentMusicIndex == 1) SetMusicVolume(game.playlist[1], game.musicVolume2);
				else if (game.currentMusicIndex == 2) SetMusicVolume(game.playlist[2], game.musicVolume3);
				else if (game.currentMusicIndex == 3) SetMusicVolume(game.playlist[3], game.musicVolume4);
				else if (game.currentMusicIndex == 4) SetMusicVolume(game.playlist[4], game.musicVolume5);
				else if (game.currentMusicIndex == 5) SetMusicVolume(game.playlist[5], game.musicVolume6);
				else if (game.currentMusicIndex == 6) SetMusicVolume(game.playlist[6], game.musicVolume7);
				else if (game.currentMusicIndex == 7) SetMusicVolume(game.playlist[7], game.musicVolume8);
			}
		}

		if (IsKeyPressed(KEY_W)) {
			std::lock_guard<std::mutex> lk(g_audioMutex);
			if (g_musicStarted.load()) {
				if (game.currentMusicIndex == 0) { game.musicVolume1 = min(game.musicVolume1 + 0.1f, 1.0f); SetMusicVolume(game.playlist[0], game.musicVolume1); }
				else if (game.currentMusicIndex == 1) { game.musicVolume2 = min(game.musicVolume2 + 0.1f, 1.0f); SetMusicVolume(game.playlist[1], game.musicVolume2); }
				else if (game.currentMusicIndex == 2) { game.musicVolume3 = min(game.musicVolume3 + 0.1f, 1.0f); SetMusicVolume(game.playlist[2], game.musicVolume3); }
				else if (game.currentMusicIndex == 3) { game.musicVolume4 = min(game.musicVolume4 + 0.1f, 1.0f); SetMusicVolume(game.playlist[3], game.musicVolume4); }
				else if (game.currentMusicIndex == 4) { game.musicVolume5 = min(game.musicVolume5 + 0.1f, 1.0f); SetMusicVolume(game.playlist[4], game.musicVolume5); }
				else if (game.currentMusicIndex == 5) { game.musicVolume6 = min(game.musicVolume6 + 0.1f, 1.0f); SetMusicVolume(game.playlist[5], game.musicVolume6); }
				else if (game.currentMusicIndex == 6) { game.musicVolume7 = min(game.musicVolume7 + 0.1f, 1.0f); SetMusicVolume(game.playlist[6], game.musicVolume7); }
				else if (game.currentMusicIndex == 7) { game.musicVolume8 = min(game.musicVolume8 + 0.1f, 1.0f); SetMusicVolume(game.playlist[7], game.musicVolume8); }
			}
		}

		if (IsKeyPressed(KEY_S)) {
			std::lock_guard<std::mutex> lk(g_audioMutex);
			if (g_musicStarted.load()) {
				if (game.currentMusicIndex == 0) { game.musicVolume1 = max(game.musicVolume1 - 0.1f, 0.0f); SetMusicVolume(game.playlist[0], game.musicVolume1); }
				else if (game.currentMusicIndex == 1) { game.musicVolume2 = max(game.musicVolume2 - 0.1f, 0.0f); SetMusicVolume(game.playlist[1], game.musicVolume2); }
				else if (game.currentMusicIndex == 2) { game.musicVolume3 = max(game.musicVolume3 - 0.1f, 0.0f); SetMusicVolume(game.playlist[2], game.musicVolume3); }
				else if (game.currentMusicIndex == 3) { game.musicVolume4 = max(game.musicVolume4 - 0.1f, 0.0f); SetMusicVolume(game.playlist[3], game.musicVolume4); }
				else if (game.currentMusicIndex == 4) { game.musicVolume5 = max(game.musicVolume5 - 0.1f, 0.0f); SetMusicVolume(game.playlist[4], game.musicVolume5); }
				else if (game.currentMusicIndex == 5) { game.musicVolume6 = max(game.musicVolume6 - 0.1f, 0.0f); SetMusicVolume(game.playlist[5], game.musicVolume6); }
				else if (game.currentMusicIndex == 6) { game.musicVolume7 = max(game.musicVolume7 - 0.1f, 0.0f); SetMusicVolume(game.playlist[6], game.musicVolume7); }
				else if (game.currentMusicIndex == 7) { game.musicVolume8 = max(game.musicVolume8 - 0.1f, 0.0f); SetMusicVolume(game.playlist[7], game.musicVolume8); }
			}
		}

		if (game.mode == 2) {
			int baseLength = 3;
			float factor = 0.03f;
			int extra = (int)game.snake.body.size() - baseLength;
			if (extra < 0) extra = 0;

			const float minInterval = 0.04f;
			float interval = 0.2f / (1.0f + extra * factor);
			if (interval < minInterval) interval = minInterval;
			game.moveInterval = interval;
		}

		game.particleSystem.Update(deltaTime);
		if (eventTriggered(game.moveInterval) && game.running && !game.paused) {
			game.Update(deltaTime);
		}

		if (game.running && !game.paused) {
			if (IsKeyPressed(KEY_UP) && game.snake.direction.y != 1) game.snake.nextDirection = { 0, -1 };
			else if (IsKeyPressed(KEY_DOWN) && game.snake.direction.y != -1) game.snake.nextDirection = { 0, 1 };
			else if (IsKeyPressed(KEY_LEFT) && game.snake.direction.x != 1) game.snake.nextDirection = { -1, 0 };
			else if (IsKeyPressed(KEY_RIGHT) && game.snake.direction.x != -1) game.snake.nextDirection = { 1, 0 };
		}

		if (game.running && IsKeyPressed(KEY_ESCAPE)) game.Pause();

		if (game.paused) {
			if (IsKeyPressed(KEY_ENTER)) game.Pause();
			if (IsKeyPressed(KEY_M)) game.ReturnToMenu();
			if (IsKeyPressed(KEY_E)) {
				std::lock_guard<std::mutex> lk(g_audioMutex);
				if (g_musicStarted.load() && !game.playlist.empty()) StopMusicStream(game.playlist[game.currentMusicIndex]);
				g_musicStarted.store(false);
				shouldExit = true;
			}
		}

		if (!game.running && IsKeyPressed(KEY_M)) game.ReturnToMenu();

		BeginDrawing();
		if (game.menuActive || game.modeSelectionActive) {
			DrawGradientBackground();
			ClearBackground(green);
		}
		else {
			if (game.mode == 2) ClearBackground(darkRed);
			else ClearBackground(green);
		}

		if (game.menuActive) {
			rlImGuiBegin();
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.9f));
			ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f),
				ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(600, 500));
			ImGui::Begin("MainMenu", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoCollapse);
			ImGui::SetWindowFontScale(2.0f);
			ImVec2 textSize = ImGui::CalcTextSize("Snake!");
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - textSize.x) * 0.5f);
			ImGui::TextColored(ImVec4(0.2f, 0.8f, 0.4f, 1.0f), "Snake!");
			ImGui::SetWindowFontScale(1.0f);
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.7f, 0.3f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.8f, 0.4f, 1.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(20, 15));
			ImGui::SetWindowFontScale(1.5f);
			ImGui::Dummy(ImVec2(0, 35));

			if (ImGui::Button("Start Game", ImVec2(-FLT_MIN, 0))) {
				game.menuActive = false;
				game.modeSelectionActive = true;
			}
			ImGui::Dummy(ImVec2(0, 15));
			if (ImGui::Button("Game Instructions", ImVec2(-FLT_MIN, 0))) game.showInstructions = true;
			ImGui::Dummy(ImVec2(0, 15));
			if (ImGui::Button("Acknowledgement", ImVec2(-FLT_MIN, 0))) game.showAcknowledgement = true;
			ImGui::Dummy(ImVec2(0, 15));

			if (ImGui::Button("Exit Game", ImVec2(-FLT_MIN, 0))) {
				std::lock_guard<std::mutex> lk(g_audioMutex);
				if (g_musicStarted.load() && !game.playlist.empty()) StopMusicStream(game.playlist[game.currentMusicIndex]);
				g_musicStarted.store(false);
				shouldExit = true;
			}

			ImGui::SetWindowFontScale(1.0f);
			ImGui::PopStyleVar(2);
			ImGui::PopStyleColor(2);

			if (game.showInstructions) {
				ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f),
					ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(820, 640));
				ImGui::Begin("Game Instructions", &game.showInstructions,
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowFontScale(1.2f);
				ImGui::TextColored(ImVec4(0.8f, 1.0f, 0.2f, 1.0f), "Controls:");
				ImGui::BulletText("Use Arrow Keys to move the snake");
				ImGui::BulletText("Press ESC to pause game");
				ImGui::BulletText("Press M to return to Main Menu");
				ImGui::BulletText("Press E to exit game");
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.8f, 1.0f, 0.2f, 1.0f), "Background Music:");
				ImGui::BulletText("Press [Space] to pause/resume music");
				ImGui::BulletText("Press A to play previous music");
				ImGui::BulletText("Press D to play next music");
				ImGui::BulletText("Press W to increase current music volume");
				ImGui::BulletText("Press S to decrease current music volume");
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.8f, 1.0f, 0.2f, 1.0f), "Game Rules:");
				ImGui::BulletText("Collect food to grow longer");
				ImGui::BulletText("Avoid hitting walls, obstacles or yourself");
				ImGui::BulletText("Each food gives 1 point");
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.8f, 1.0f, 0.2f, 1.0f), "Hard Mode:");
				ImGui::BulletText("Random obstacles");
				ImGui::BulletText("The longer you grow, the faster you move");
				if (ImGui::Button("Close", ImVec2(-FLT_MIN, 60))) {
					game.showInstructions = false;
				}
				ImGui::SetWindowFontScale(1.0f);
				ImGui::End();
			}

			if (game.showAcknowledgement) {
				ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f),
					ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				ImGui::SetNextWindowSize(ImVec2(640, 450));
				ImGui::Begin("Acknowledgement", &game.showAcknowledgement,
					ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
				ImGui::SetWindowFontScale(1.2f);
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "Music:");
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "a brief love affair - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "winter heartbeat - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "quite rooms - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "la vie en rose - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "Lovers - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "be there - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "Strawberry Kisses - Olivia Herdt");
				ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.8f, 1.0f), "love galore - Olivia Herdt");
				/*ImGui::Separator();
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.5f, 0.9f, 1.0f, 1.0f), "Special thanks to:");
				ImGui::TextColored(ImVec4(0.5f, 0.9f, 1.0f, 1.0f), "LCQ Liza YSJ XHR HKW Evgenia");*/
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Contact me :)");
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "https://github.com/LCZ-ctrl");
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::TextColored(ImVec4(0.1f, 1.0f, 0.4f, 1.0f), "No commercial use!!");
				ImGui::Spacing();
				if (ImGui::Button("Close", ImVec2(-FLT_MIN, 40))) game.showAcknowledgement = false;
				ImGui::SetWindowFontScale(1.0f);
				ImGui::End();
			}

			ImGui::End();
			ImGui::PopStyleColor();
			rlImGuiEnd();
		}
		else if (game.modeSelectionActive) {
			rlImGuiBegin();
			int highScore1 = GetHighScoreForMode(1);
			int highScore2 = GetHighScoreForMode(2);
			ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.15f, 0.15f, 0.15f, 0.9f));
			ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f),
				ImGuiCond_Always, ImVec2(0.5f, 0.5f));
			ImGui::SetNextWindowSize(ImVec2(700, 460));
			ImGui::Begin("Mode Selection", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse);
			ImGui::SetWindowFontScale(2.0f);
			ImVec2 modeTextSize = ImGui::CalcTextSize("Select Game Mode");
			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - modeTextSize.x) * 0.5f);
			ImGui::Text("Select Game Mode");
			ImGui::Spacing();
			ImGui::SetWindowFontScale(1.5f);
			ImGui::Dummy(ImVec2(0, 40));
			if (ImGui::Button("Wanna Sweat", ImVec2(-FLT_MIN, 0))) {
				game.mode = 1;
				game.food.isHardMode = false;
				game.modeSelectionActive = false;
				game.running = true;
			}
			ImGui::Dummy(ImVec2(0, 5));
			if (highScore1 >= 0) ImGui::Text("Best Record: %d", highScore1); else ImGui::Text("Best Record: No Record");
			ImGui::Dummy(ImVec2(0, 25));
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.7f, 0.0f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.2f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.4f, 0.0f, 0.0f, 1.0f));
			if (ImGui::Button("Wanna Bleed", ImVec2(-FLT_MIN, 0))) {
				game.mode = 2;
				game.food.isHardMode = true;
				game.modeSelectionActive = false;
				game.running = true;
				game.GenerateObstacles();
			}
			ImGui::PopStyleColor(3);
			ImGui::Dummy(ImVec2(0, 5));
			if (highScore2 >= 0) ImGui::Text("Best Record: %d", highScore2); else ImGui::Text("Best Record: No Record");
			ImGui::Dummy(ImVec2(0, 25));
			ImGui::Spacing();
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.7f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.8f, 0.0f, 1.0f));
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.6f, 0.0f, 1.0f));
			if (ImGui::Button("Return to Main Menu", ImVec2(-FLT_MIN, 0))) {
				game.modeSelectionActive = false;
				game.menuActive = true;
				if (!g_musicStarted.load()) {
					std::lock_guard<std::mutex> lk(g_audioMutex);
					if (!game.playlist.empty()) {
						PlayMusicStream(game.playlist[game.currentMusicIndex]);
						if (game.currentMusicIndex == 0) SetMusicVolume(game.playlist[0], game.musicVolume1);
						else if (game.currentMusicIndex == 1) SetMusicVolume(game.playlist[1], game.musicVolume2);
						else if (game.currentMusicIndex == 2) SetMusicVolume(game.playlist[2], game.musicVolume3);
						else if (game.currentMusicIndex == 3) SetMusicVolume(game.playlist[3], game.musicVolume4);
						else if (game.currentMusicIndex == 4) SetMusicVolume(game.playlist[4], game.musicVolume5);
						else if (game.currentMusicIndex == 5) SetMusicVolume(game.playlist[5], game.musicVolume6);
						g_musicStarted.store(true);
						game.musicPaused = false;
					}
				}
			}
			ImGui::PopStyleColor(3);
			ImGui::SetWindowFontScale(1.0f);
			ImGui::End();
			ImGui::PopStyleColor();
			rlImGuiEnd();
		}
		else {
			DrawRectangleLinesEx(
				Rectangle{ (float)offset - 5, (float)offset - 5,
						   (float)cellSize * cellCount + 10, (float)cellSize * cellCount + 10 },
				5, darkGreen
			);
			DrawText("Snake!", offset - 5, 20, 40, darkGreen);
			DrawText("Score:", offset + cellSize * cellCount - 160, 20, 40, darkGreen);
			if (game.running) DrawText(TextFormat("%i", game.score), offset + cellSize * cellCount - 20, 20, 40, darkGreen);
			else DrawText(TextFormat("%i", game.finalScore), offset + cellSize * cellCount - 20, 20, 40, darkGreen);
			game.Draw();
		}
		EndDrawing();
	}

	{
		std::lock_guard<std::mutex> lk(g_audioMutex);
		if (g_musicStarted.load() && !game.playlist.empty()) {
			StopMusicStream(game.playlist[game.currentMusicIndex]);
			g_musicStarted.store(false);
		}
	}
	g_audioThreadRunning = false;
	if (g_audioThread.joinable()) g_audioThread.join();

	rlImGuiShutdown();
	CloseWindow();
	return 0;
}