#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include "MainMenu.h"
#include "InputSystem.h"
#include "Transition.h"
#include "Screen.h"
#include "hangman.h"
#include "Snakegame.h"
#include "Wordle.h"
// #include "optionmenu.h"
using namespace std;

class Gameboy
{
private:
    // Player player;
    Font font;
    Screen screen;
    InputSystem inputSystem;
    // SoundSystem soundSystem;
    // Menu menu;
    Leaderboard leaderboard;
    bool transitionShown;
    MenuSystem mainMenu;
    MenuSystem gameMenu;
    SoundSystem soundSystem;

    bool isMainMenuActive = true;
    bool isHangmanMenuActive = false;
    bool isSnakeMenuActive = false;
    bool isWordlyMenuActive = false;
    bool isPlayingHangman = false;
    bool isPlayingSnake = false;
    bool isplayingwordle = false;
    bool isoptionsnake = false;
    bool isoptionwordle = false;
    bool isoptionhangman = false;
    bool ishowtoplaysnake = false;
    bool ishowtoplayhangman = false;
    bool ishowtoplaywordle = false;
    bool isLearboardselected = false;
    bool hasEnteredName = false;
    bool issnakeinstruction = false;
    bool iswordleinstruction = false;
    bool ishangmaninstruction = false;

    string playerName;
    Text nameText;

    void initializeFont()
    {
        if (!font.loadFromFile("Fonts/arial/arial.ttf"))
        {
            cerr << "Error loading font!" << endl;
            return;
        }
        nameText.setFont(font);
        nameText.setCharacterSize(40);
        nameText.setFillColor(Color::Black);
        nameText.setPosition(400, 300);
        playerName = "";
    }

    void inputPlayerName(RenderWindow &window)
    {
        Text instructionText("Enter your name: ", font, 30);
        instructionText.setFillColor(Color::Black);
        instructionText.setPosition(400, 250);

        bool nameEntered = false;

        while (!nameEntered && window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                else if (event.type == Event::TextEntered)
                {
                    char inputChar = static_cast<char>(event.text.unicode);
                    if ((inputChar >= 'a' && inputChar <= 'z') || (inputChar >= 'A' && inputChar <= 'Z'))
                    {
                        playerName += inputChar;
                        nameText.setString(playerName);
                    }
                    else if (inputChar == '\b' && !playerName.empty())
                    {
                        playerName.pop_back();
                        nameText.setString(playerName);
                    }
                }
                else if (event.type == Event::KeyPressed && event.key.code == Keyboard::Return && !playerName.empty())
                {
                    nameEntered = true;
                }
            }

            window.clear(Color::White);
            window.draw(instructionText);
            window.draw(nameText);
            window.display();
        }
    }

    void renderLeaderboard(RenderWindow &window)
    {
        // Force reload scores from file
        leaderboard.loadScores();

        Text title("Leaderboard", font, 50);
        title.setFillColor(Color::Black);

        // Center the title
        FloatRect titleBounds = title.getLocalBounds();
        title.setOrigin(titleBounds.width / 2, titleBounds.height / 2);
        title.setPosition(window.getSize().x / 2, 50);

        vector<Text> scoreTexts;
        const float startY = 150;
        const float padding = 50;

        // Display each score entry
        for (int i = 0; i < leaderboard.getCurrentSize(); ++i)
        {
            try
            {
                const Leaderboard::ScoreEntry &entry = leaderboard.getScoreEntry(i);

                Text scoreText;
                scoreText.setFont(font);
                stringstream ss;
                ss << (i + 1) << ". " << entry.name << " - " << entry.score;
                scoreText.setString(ss.str());
                scoreText.setCharacterSize(30);
                scoreText.setFillColor(Color::Black);

                // Center each score entry
                FloatRect textBounds = scoreText.getLocalBounds();
                scoreText.setOrigin(textBounds.width / 2, textBounds.height / 2);
                scoreText.setPosition(window.getSize().x / 2, startY + i * padding);

                scoreTexts.push_back(scoreText);
            }
            catch (const out_of_range &e)
            {
                cerr << "Error accessing leaderboard entry: " << e.what() << endl;
            }
        }

        Text exitText("Press ESC to return to Main Menu", font, 25);
        exitText.setFillColor(Color::Black);
        FloatRect exitBounds = exitText.getLocalBounds();
        exitText.setOrigin(exitBounds.width / 2, exitBounds.height / 2);
        exitText.setPosition(window.getSize().x / 2, startY + leaderboard.getCurrentSize() * padding + 30);

        // Display loop
        bool exitLeaderboard = false;
        while (!exitLeaderboard && window.isOpen())
        {
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    window.close();
                }
                if (event.type == Event::KeyPressed && event.key.code == Keyboard::Escape)
                {
                    exitLeaderboard = true;
                }
            }

            window.clear(Color::White);
            window.draw(title);
            for (const auto &text : scoreTexts)
            {
                window.draw(text);
            }
            window.draw(exitText);
            window.display();
        }
    }

public:
    Gameboy() : transitionShown(false), mainMenu(1200.f, 800.f, true), gameMenu(1200.f, 800.f, false),
                isMainMenuActive(true),
                isSnakeMenuActive(false),
                isPlayingSnake(false),
                isWordlyMenuActive(false),
                isPlayingHangman(false),
                isHangmanMenuActive(false), isplayingwordle(false), leaderboard("leaderboard.txt")
    {
        initializeFont();
        inputSystem.mapKey(Keyboard::Up, "Up");
        inputSystem.mapKey(Keyboard::Down, "Down");
        inputSystem.mapKey(Keyboard::Escape, "Escape");
        inputSystem.mapKey(Keyboard::Return, "Enter");
        inputSystem.mapKey(Keyboard::Left, "Left");
        inputSystem.mapKey(Keyboard::Right, "Right");
    }

    void startGame()
    {
        Sprite background, wordelback, snackback;
        Texture t1, t2, t3;

        if (!t1.loadFromFile("hangmanpic/back.jpg"))
        {
            cout << "Error loading texture" << endl;
            return;
        }
        background.setTexture(t1);

        if (!t2.loadFromFile("Wordlegamepic/wordle_11zon.jpg"))
        {
            cout << "Error loading texture" << endl;
            return;
        }
        wordelback.setTexture(t2);

        if (!t3.loadFromFile("Snakegamepic/snackback_11zon.jpg"))
        {
            cout << "Error loading texture" << endl;
            return;
        }
        snackback.setTexture(t3);

        if (!showNintendoTransition(screen.window))
        {
            return;
        }
        while (screen.window.isOpen())
        {
            Event event;
            while (screen.window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    screen.window.close();
                }
            }

            inputSystem.handleInput();

            if (isMainMenuActive)
            {
                if (inputSystem.isActionActive("Up"))
                {
                    mainMenu.MoveUp();
                }
                else if (inputSystem.isActionActive("Down"))
                {
                    mainMenu.MoveDown();
                }
                else if (inputSystem.isActionActive("Enter"))
                {
                    int selectedItem = mainMenu.getSelectedItem();
                    if (selectedItem == 0)
                    {
                        isMainMenuActive = false;
                        isSnakeMenuActive = true;
                    }
                    else if (selectedItem == 1)
                    {
                        isMainMenuActive = false;
                        isWordlyMenuActive = true;
                    }
                    else if (selectedItem == 2)
                    {
                        isMainMenuActive = false;
                        isHangmanMenuActive = true;
                    }
                    else if (selectedItem == 3)
                    {
                        cout << "Leaderboard is selected" << endl;
                        isLearboardselected = true;
                        isMainMenuActive = false;
                    }
                    else if (selectedItem == 4)
                    {
                        screen.window.close();
                    }
                }
            }
            else if (isHangmanMenuActive || isSnakeMenuActive || isWordlyMenuActive)
            {
                if (inputSystem.isActionActive("Up"))
                {
                    gameMenu.MoveUp();
                }
                else if (inputSystem.isActionActive("Down"))
                {
                    gameMenu.MoveDown();
                }
                else if (inputSystem.isActionActive("Enter"))
                {
                    int selectedItem = gameMenu.getSelectedItem();
                    if (selectedItem == 0)
                    {
                        if (isHangmanMenuActive)
                        {
                            isHangmanMenuActive = false;
                            isPlayingHangman = true;
                            isoptionsnake = false;
                            isoptionwordle = false;
                            isoptionhangman = false;
                        }
                        else if (isSnakeMenuActive)
                        {
                            isSnakeMenuActive = false;
                            isPlayingSnake = true;
                            isoptionsnake = false;
                            isoptionwordle = false;
                            isoptionhangman = false;
                        }
                        else if (isWordlyMenuActive)
                        {
                            isWordlyMenuActive = false;
                            isplayingwordle = true;
                            isoptionsnake = false;
                            isoptionwordle = false;
                            isoptionhangman = false;
                        }
                    }

                    else if (selectedItem == 1)
                    {
                        cout << "load game is selected" << endl;
                    }

                    else if (selectedItem == 2)
                    {
                        if (isHangmanMenuActive)
                        {
                            ishangmaninstruction = true;
                            isHangmanMenuActive = false;
                            gameMenu.setInstruction("Hangman Instructions:\nGuess the word by typing letters.\nYou have 6 lives. Good luck!");
                        }
                        else if (isSnakeMenuActive)
                        {
                            issnakeinstruction = true;
                            isSnakeMenuActive = false;
                            gameMenu.setInstruction("Snake Game Instructions:\nUse arrow keys to control the snake.\nAvoid hitting walls or yourself.");
                        }
                        else if (isWordlyMenuActive)
                        {
                            iswordleinstruction = true;
                            isWordlyMenuActive = false;
                            gameMenu.setInstruction("Wordly Instructions:\nGuess the correct word within 6 tries.\nEach guess provides feedback.");
                        }
                    }
                    else if (selectedItem == 3)
                    {
                        isHangmanMenuActive = false;
                        isSnakeMenuActive = false;
                        isWordlyMenuActive = false;
                        isMainMenuActive = true;
                    }
                    else if (selectedItem == 4)
                    {
                        screen.window.close();
                    }
                }
                else if (inputSystem.isActionActive("Escape"))
                {
                    isHangmanMenuActive = false;
                    isSnakeMenuActive = false;
                    isWordlyMenuActive = false;
                    isMainMenuActive = true;
                }
            }

            screen.window.clear(Color::White);

            if (isMainMenuActive)
            {
                mainMenu.draw(screen.window);
            }
            else if (isHangmanMenuActive)
            {
                if (!hasEnteredName)
                {
                    inputPlayerName(screen.window);
                    hasEnteredName = true;
                }
                screen.window.draw(background);
                gameMenu.draw(screen.window);
            }
            else if (isSnakeMenuActive)
            {
                if (!hasEnteredName)
                {
                    inputPlayerName(screen.window);
                    hasEnteredName = true;
                }
                screen.window.draw(snackback);
                gameMenu.draw(screen.window);
            }
            else if (isWordlyMenuActive)
            {
                if (!hasEnteredName)
                {
                    inputPlayerName(screen.window);
                    hasEnteredName = true;
                }
                screen.window.draw(wordelback);
                gameMenu.draw(screen.window);
            }

            else if (ishangmaninstruction)
            {
                screen.window.clear(Color::White);
                gameMenu.setInstruction("Hangman Instructions:\nGuess the word by typing letters.\nYou have 6 lives. Good luck!");
                gameMenu.toggleInstructions();
                gameMenu.draw(screen.window);
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    ishangmaninstruction = false;
                    isHangmanMenuActive = true;
                    gameMenu.toggleInstructions();
                }
            }
            else if (issnakeinstruction)
            {
                screen.window.clear(Color::White);
                gameMenu.setInstruction("Snake Game Instructions:\nUse arrow keys to control the snake.\nAvoid hitting walls or yourself.");
                gameMenu.toggleInstructions();
                gameMenu.draw(screen.window);
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    issnakeinstruction = false;
                    isSnakeMenuActive = true;
                    gameMenu.toggleInstructions();
                }
            }
            else if (iswordleinstruction)
            {
                screen.window.clear(Color::White);
                gameMenu.setInstruction("Wordly Instructions:\nGuess the correct word within 6 tries.\nEach guess provides feedback.");
                gameMenu.toggleInstructions();
                gameMenu.draw(screen.window);
                if (Keyboard::isKeyPressed(Keyboard::Escape))
                {
                    iswordleinstruction = false;
                    isWordlyMenuActive = true;
                    gameMenu.toggleInstructions();
                }
            }

            else if (isPlayingHangman)
            {
                HangmanGame hangman;
                hangman.run(screen.window);
                isPlayingHangman = false;
                isHangmanMenuActive = true;
            }
            else if (isPlayingSnake)
            {
                SnakeGame snake;
                snake.run(screen.window);
                isPlayingSnake = false;
                isSnakeMenuActive = true;
            }
            else if (isplayingwordle)
            {
                WordleGame wordle;
                wordle.startGame(screen.window);
                isplayingwordle = false;
                isWordlyMenuActive = true;
            }
            else if (isLearboardselected)
            {
                renderLeaderboard(screen.window);
                isLearboardselected = false;
                isMainMenuActive = true;
            }

            screen.window.display();
        }
    }
};