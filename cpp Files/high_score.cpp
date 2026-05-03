#include "high_score.h"
#include <fstream>
#include <direct.h>

const std::string highScoreFolder = "SnakeRecords";
const std::string mode1File = highScoreFolder + "\\mode1.txt";
const std::string mode2File = highScoreFolder + "\\mode2.txt";

void CreateHighScoreFolder() {
    struct stat info;
    if (stat(highScoreFolder.c_str(), &info) != 0) {
        _mkdir(highScoreFolder.c_str());
    }
}

int GetHighScoreForMode(int mode) {
    std::string filePath = (mode == 1 ? mode1File : mode2File);
    std::ifstream in(filePath);
    int highScore;
    if (in >> highScore) {
        return highScore; // return the stored record
    }
    return -1;
}

void SetHighScoreForMode(int mode, int score) {
    CreateHighScoreFolder();

    std::string filePath = (mode == 1 ? mode1File : mode2File);
    std::ofstream out(filePath);
    if (out.is_open()) {
        out << score; // write in the new record
        out.close();
    }
}
