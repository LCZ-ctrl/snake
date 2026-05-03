#pragma once
#include <string>

extern const std::string highScoreFolder;
extern const std::string mode1File;
extern const std::string mode2File;

void CreateHighScoreFolder();
int GetHighScoreForMode(int mode);
void SetHighScoreForMode(int mode, int score);
