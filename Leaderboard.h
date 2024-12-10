#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Leaderboard {
public: // Move ScoreEntry to the public section
    struct ScoreEntry {
        string name;
        int score;
    };

private:
    static const int MAX_ENTRIES = 10;
    string filename;
    ScoreEntry scores[MAX_ENTRIES];
    int currentSize = 0; // Number of valid entries

    void sortScores() {
        for (int i = 0; i < currentSize - 1; ++i) {
            for (int j = 0; j < currentSize - i - 1; ++j) {
                if (scores[j].score < scores[j + 1].score) {
                    ScoreEntry temp = scores[j];
                    scores[j] = scores[j + 1];
                    scores[j + 1] = temp;
                }
            }
        }
    }

public:
    Leaderboard(const string& file = "leaderboard.txt") : filename(file) {
        loadScores();
    }

    void loadScores() {
        currentSize = 0; // Reset size
        ifstream fileIn(filename);
        if (fileIn.is_open()) {
            string name;
            int score;
            while (fileIn >> name >> score && currentSize < MAX_ENTRIES) {
                scores[currentSize].name = name;
                scores[currentSize].score = score;
                ++currentSize;
            }
            fileIn.close();
        }
        sortScores();
    }

    void saveScores() {
        ofstream fileOut(filename, ios::trunc);
        if (fileOut.is_open()) {
            for (int i = 0; i < currentSize; ++i) {
                fileOut << scores[i].name << " " << scores[i].score << "\n";
            }
            fileOut.close();
        }
    }

    void addScore(const string& playerName, int score) {
        if (currentSize < MAX_ENTRIES) {
            scores[currentSize].name = playerName;
            scores[currentSize].score = score;
            ++currentSize;
        }
        else if (score > scores[MAX_ENTRIES - 1].score) { // Replace if the new score is higher
            scores[MAX_ENTRIES - 1].name = playerName;
            scores[MAX_ENTRIES - 1].score = score;
        }
        sortScores();
        saveScores();
    }

    void displayScores() const {
        cout << "Leaderboard:\n";
        for (int i = 0; i < currentSize; ++i) {
            cout << i + 1 << ". " << scores[i].name << " - " << scores[i].score << "\n";
        }
    }

    int getCurrentSize() const {
        return currentSize;
    }

    const ScoreEntry* getScores() const {
        return scores;
    }
    const ScoreEntry& getScoreEntry(int index) const {
        if (index >= 0 && index < currentSize) {
            return scores[index];
        }
        throw out_of_range("Index out of bounds");
    }
};