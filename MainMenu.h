#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class MenuSystem {
private:
    int selectedItemIndex;
    Font font;
    Text menuItems[6];
    int menuItemCount;
    //bool isOptionsMenu;// Track if we're in the options menu
    Sprite background;
    Texture back;
    bool isMainMenu;
    bool howtoplay;
    Text instruction;
    //bool vfxEnabled; // Option states


    const char* instructions[5]; // Array to store instructions for each menu item.

public:
    MenuSystem(float width, float height, bool isMain = true)
        : selectedItemIndex(0), isMainMenu(isMain), menuItemCount(0), howtoplay(false) {
        if (!font.loadFromFile("Fonts/arial/arial.ttf")) {
            cout << "Error loading font" << endl;
            return;
        }

        if (isMain) {
            if (!back.loadFromFile("Gameboypic/Game_11zon.png")) {
                cout << "Error loading texture" << endl;
                return;
            }
            background.setTexture(back);
        }

        const char* items[6];
        float xPosition;

        if (isMain) {
            items[0] = "Snake Game";
            items[1] = "Wordly";
            items[2] = "HangMan";
            items[3] = "Leaderboard";
            items[4] = "Exit";
            menuItemCount = 5;
            xPosition = width / 1.54f;

            // Set instructions for main menu items.
            instructions[0] = "Snake Game Instructions:\nUse arrow keys to control the snake.\nAvoid hitting walls or yourself.";
            instructions[1] = "Wordly Instructions:\nGuess the correct word within 6 tries.\nEach guess provides feedback.";
            instructions[2] = "HangMan Instructions:\nGuess the word by typing letters.\nYou have 6 lives. Good luck!";
            instructions[3] = ""; // Exit has no instructions.
        }
        //else if (isOptionsMenu) {
        //    items[0] = "VFX: ON"; // Default state
        //    items[1] = "Sound: ON"; // Default state
        //    items[2] = "Back to Main Menu";
        //    menuItemCount = 3;
        //    xPosition = width / 2.0f;
        //}
        else {
            items[0] = "Play";
            items[1] = "Load Game";
            items[2] = "How to Play";
            items[3] = "Main Menu";
            items[4] = "Exit";
            //items[5] = "";
            menuItemCount = 5;
            xPosition = width / 2.0f;
        }

        for (int i = 0; i < menuItemCount; i++) {
            menuItems[i].setFont(font);
            menuItems[i].setCharacterSize(24);
            menuItems[i].setString(items[i]);
            menuItems[i].setPosition(Vector2f(xPosition, height / (menuItemCount + 1) * (i + 1)));
            menuItems[i].setFillColor(i == 0 ? Color::Red : (isMain ? Color::Black : Color::White));
        }

        instruction.setFont(font);
        instruction.setCharacterSize(24);
        instruction.setFillColor(Color::Red);
        instruction.setPosition(50.f, 100.f);
    }

    void draw(RenderWindow& window) {
        if (isMainMenu) {
            window.draw(background);
        }

        // Only draw menu items if not showing instructions
        if (!howtoplay) {
            for (int i = 0; i < menuItemCount; i++) {
                window.draw(menuItems[i]);
            }
        }
        else {
            // Create a semi-transparent background
            RectangleShape backdrop(Vector2f(800, 400));
            backdrop.setFillColor(Color(0, 0, 0, 180));  // Black with 180/255 alpha
            backdrop.setPosition(200, 200);
            window.draw(backdrop);

            // Center and style the instruction text
            instruction.setCharacterSize(30);
            instruction.setFillColor(Color::White);
            FloatRect textBounds = instruction.getLocalBounds();
            instruction.setOrigin(textBounds.width / 2, textBounds.height / 2);
            instruction.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f);
            window.draw(instruction);

            // Add "Press ESC to return" text
            Text exitText;
            exitText.setFont(font);
            exitText.setString("Press ESC to return");
            exitText.setCharacterSize(24);
            exitText.setFillColor(Color::White);
            FloatRect exitBounds = exitText.getLocalBounds();
            exitText.setOrigin(exitBounds.width / 2, exitBounds.height / 2);
            exitText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f + 100);
            window.draw(exitText);
        }
    }




    void MoveUp() {
        if (selectedItemIndex >= 0) {
            menuItems[selectedItemIndex].setFillColor(isMainMenu ? Color::Black : Color::White);
            selectedItemIndex--;
            if (selectedItemIndex == -1) {
                selectedItemIndex = menuItemCount - 1;
            }
            menuItems[selectedItemIndex].setFillColor(Color::Red);
        }
    }

    void MoveDown() {
        if (selectedItemIndex < menuItemCount) {
            menuItems[selectedItemIndex].setFillColor(isMainMenu ? Color::Black : Color::White);
            selectedItemIndex++;
            if (selectedItemIndex == menuItemCount) {
                selectedItemIndex = 0;
            }
            menuItems[selectedItemIndex].setFillColor(Color::Red);
        }
    }

    void toggleInstructions() {
        howtoplay = !howtoplay;
    }

    //void toggleVFX() {
    //    vfxEnabled = !vfxEnabled;
    //    menuItems[0].setString(vfxEnabled ? "VFX: ON" : "VFX: OFF");
    //}

    void setInstruction(const std::string& newInstruction) {
        instruction.setString(newInstruction);
    }

    int getSelectedItem() const {
        return selectedItemIndex;
    }
};

