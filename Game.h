#pragma once
#include<iostream>
#include<string>
#include<SFML/Graphics.hpp>
#include"SoundSystem.h"
#include"InputSystem.h"
#include "Leaderboard.h"


using namespace std;
using namespace sf;

class Game {
protected:
    string name;
    int score;
    string playerName;
    Text nameTxt;
    Font font;
    bool isNameEntered;
    SoundSystem soundSystem;
    InputSystem inputsystem;
    Leaderboard leaderboard;

public:
    Game() : isNameEntered(false), leaderboard("leaderboard.txt") {
        if (!font.loadFromFile("Fonts/arial/arial.ttf")) {
            cerr << "Error loading font in Game class" << endl;
            return;
        }
        nameTxt.setFont(font);
        nameTxt.setCharacterSize(40);
        nameTxt.setFillColor(Color::Black);
        nameTxt.setPosition(400, 300);
        playerName = "";
    }
    void inputPlayerName(RenderWindow& window) {
        Text instructionText("Enter your name: ", font, 30);
        instructionText.setFillColor(Color::Black);
        instructionText.setPosition(400, 250);
        while (!isNameEntered && window.isOpen()) {
            Event evnt;
            while (window.pollEvent(evnt)) {
                if (evnt.type == Event::Closed) {
                    window.close();
                }
                else if (evnt.type == Event::TextEntered) {
                    char inputChar = static_cast<char>(evnt.text.unicode);
                    if ((inputChar >= 'a' && inputChar <= 'z') || (inputChar >= 'A' && inputChar <= 'Z')) {
                        playerName += inputChar;
                        nameTxt.setString(playerName);
                    }
                    else if (inputChar == '\b' && !playerName.empty()) {
                        playerName.pop_back();
                        nameTxt.setString(playerName);
                    }
                }
                else if (evnt.type == Event::KeyPressed && evnt.key.code == Keyboard::Return && !playerName.empty()) {
                    isNameEntered = true;
                }
            }
            window.clear(Color::White);
            window.draw(instructionText);
            window.draw(nameTxt);
            window.display();
        }
    }

    void updateLeaderboard(const string& playerName, int score) {
        leaderboard.addScore(playerName, score);
    }

    void showLeaderboard() {
        leaderboard.displayScores();
    }
};