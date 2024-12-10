#pragma once
#include<iostream>
#include <cstdlib> 
#include <ctime>   
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include"Game.h"
using namespace std;
using namespace sf;

class Category {
private:
    static const int MAX_WORDS = 100;
    static const int MAX_WORD_LENGTH = 10;
    char words[MAX_WORDS][MAX_WORD_LENGTH];

public:
    Category() {
        
        const char* wordList[MAX_WORDS] = {
            "apple", "banana", "cherry", "date", "figure", "abcd", "amazon", "abby", "blizzard", "easport",
            "fortnite", "gabe", "hello", "intel", "java", "killer", "lego", "microsoft", "nintendo", "outlook",
            "playstation", "quick", "rocket", "starcraft", "tensor", "ubisoft", "viva", "windows", "xbox",
            "yahoo", "zero", "gameboy", "battlefield", "minecraft", "assassins", "code", "reborn", "parallel",
            "shadow", "three", "more", "rebel", "blox", "combat", "handle", "cooperative", "movie", "fantasy",
            "strike", "grand", "overload", "spy", "marshal", "runner", "target", "evolve", "legend", "saga",
            "prophecy", "franchise", "shield", "league", "barrage", "gun", "detain", "sniper", "hero", "vehicle",
            "remedy", "server", "rebellion", "archer", "mania", "hunter", "fast", "fury", "panic", "flash",
            "revelation", "hell", "noise", "serenity", "guns", "detective", "bonus", "virtual", "trial",
            "tournament", "project", "destiny", "creator", "system", "player", "version", "login", "format",
            "deal", "classic", "champion", "arena"
        };

        // Initialize the word array with the word list
        for (int i = 0; i < MAX_WORDS; i++) {
            int j = 0;
            while (wordList[i][j] != '\0' && j < MAX_WORD_LENGTH - 1) {
                words[i][j] = wordList[i][j];
                j++;
            }
            words[i][j] = '\0'; 
        }
    }

    void getRandomWord(char* randomWord) {
        srand(time(0));
        
        int index = rand() % MAX_WORDS;

        
        int i = 0;
        while (words[index][i] != '\0') {
            randomWord[i] = words[index][i];
            cout << randomWord[i];
            i++;

        }
        cout << endl;
        randomWord[i] = '\0';
        
    }

};


class HangmanFigure {
private:
    int lives;
    Texture hangmanTextures[7],t1, t2;
    Sprite hangmanSprite,s1,background;
    string keyboard[3] = {
        "QWERTYUIOP",
        "ASDFGHJKL",
        "ZXCVBNM"
    };

public:

    

   

    HangmanFigure() : lives(6) {
        
        const string fileNames[] = {"hangmanpic/pics/bg7.png","hangmanpic/pics/bg6.png","hangmanpic/pics/bg5.png","hangmanpic/pics/bg4.png","hangmanpic/pics/bg3.png","hangmanpic/pics/bg2.png","hangmanpic/pics/bg1.png"
        };

        for (int i = 0; i < 7; i++) {
            if (!hangmanTextures[i].loadFromFile(fileNames[i])) {
                cerr << "Error loading hangman texture " << fileNames[i] << endl;
            }
            if (!t1.loadFromFile("hangmanpic/hangman.png")) {
                cerr << "Error loading hangman texture " << fileNames[i] << endl;
            }

        }
         if (!t1.loadFromFile("hangmanpic/back.png")) {
        cout << "Error loading texture" << endl;
        return;
    }
        background.setTexture(t2);
        hangmanSprite.setTexture(hangmanTextures[6]);  
        s1.setTexture(t1);
        s1.setTextureRect(IntRect(1498, 782, 270, 328));
    }

    void draw(RenderWindow& window, const string& playerName) {
        if (playerName == "Sayori" && lives == 0) {
            window.draw(s1);  
        }
        else {
            hangmanSprite.setTexture(hangmanTextures[lives]);
            window.draw(hangmanSprite);  
        }
    }

    void decreaseLives() {
        if (lives > 0) lives--;
    }

    int getLives() const {
        return lives;
    }
};

class HangmanGame : public Game {
private:
    static const int MAX_WORD_LENGTH = 20;
    char word[MAX_WORD_LENGTH];
    char hiddenWord[MAX_WORD_LENGTH];
    char guessedLetters[26];
    int guessedCount;
    int wordLength;
    HangmanFigure hangmanFigure;
    Category category;
    int score;
    bool isgameend;


    //SoundSystem soundSystem; // Add SoundSystem 

    
    Font font;
    Text wordText;
    Text guessedText;
    Text messageText;
    Text livesText;
    Text instructionText;
    Text keyboardText;
    Text scoreText;

    void initializeText() {
        if (!font.loadFromFile("Fonts/arial/arial.ttf")) {
            cerr << "Error loading font" << endl;
            return;
        }

        // Initialize text objects
        wordText.setFont(font);
        wordText.setCharacterSize(30);
        wordText.setFillColor(Color::Black);
        wordText.setPosition(800, 200);

        guessedText.setFont(font);
        guessedText.setCharacterSize(10);
        guessedText.setFillColor(Color::Blue);
        guessedText.setPosition(800, 230);

        messageText.setFont(font);
        messageText.setCharacterSize(20);
        messageText.setFillColor(Color::Black);
        messageText.setPosition(800, 260);

        livesText.setFont(font);
        livesText.setCharacterSize(20);
        livesText.setFillColor(Color::Black);
        livesText.setPosition(800, 290);

        instructionText.setFont(font);
        instructionText.setCharacterSize(15);
        instructionText.setFillColor(Color::Black);
        instructionText.setPosition(800, 330);
        instructionText.setString("Press ESC to return to menu\nType a letter to guess");

        keyboardText.setFont(font);
        keyboardText.setCharacterSize(40);
        keyboardText.setFillColor(Color::Black);
        keyboardText.setPosition(600, 440);
        keyboardText.setString("Q  W  E  R  T  Y  U  I  O  P\n"
            " A  S  D  F  G  H  J  K  L\n"
            "  Z  X  C  V  B  N  M");


        scoreText.setFont(font);
        scoreText.setCharacterSize(35);  
        scoreText.setFillColor(Color::Black);
        scoreText.setPosition(400, 20);     
        scoreText.setString("Score: 0");
    }

public:
    HangmanGame() : guessedCount(0), score(0) {
        category.getRandomWord(word);

        wordLength = 0;
        while (word[wordLength] != '\0') {
            wordLength++;
        }

        for (int i = 0; i < wordLength; i++) {
            hiddenWord[i] = '-';
        }
        hiddenWord[wordLength] = '\0';

        for (int i = 0; i < 26; i++) {
            guessedLetters[i] = '\0';
        }

        initializeText();
        inputsystem.mapKey(Keyboard::Up, "Up");
        inputsystem.mapKey(Keyboard::Down, "Down");
        inputsystem.mapKey(Keyboard::Left, "Left");
        inputsystem.mapKey(Keyboard::Right, "Right");
        inputsystem.mapKey(Keyboard::Return, "Enter");
    }

    //reset game 
    void resetGame() {
        
        category.getRandomWord(word);

        
        wordLength = 0;
        while (word[wordLength] != '\0') {
            wordLength++;
        }

        
        for (int i = 0; i < wordLength; i++) {
            hiddenWord[i] = '-';
        }
        hiddenWord[wordLength] = '\0';

        
        guessedCount = 0;
        for (int i = 0; i < 26; i++) {
            guessedLetters[i] = '\0';
        }

        
        hangmanFigure = HangmanFigure();

        
        score = 0;
        messageText.setString("");
    }

    void draw(RenderWindow& window) {

        scoreText.setString("Score: " + to_string(score));

        string displayWord;
        for (int i = 0; i < wordLength; i++) {
            displayWord += hiddenWord[i];
            displayWord += ' ';
        }
        wordText.setString("Word: " + displayWord);

        string guessedDisplay = "Guessed letters: ";
        for (int i = 0; i < guessedCount; i++) {
            guessedDisplay += guessedLetters[i];
            guessedDisplay += ' ';
        }
        guessedText.setString(guessedDisplay);
        livesText.setString("Lives remaining: " + to_string(hangmanFigure.getLives()));

        hangmanFigure.draw(window, playerName);
        window.draw(scoreText);
        //window.draw(playerNameText);
        window.draw(wordText);
        window.draw(guessedText);
        window.draw(messageText);
        window.draw(livesText);
        window.draw(instructionText);
        window.draw(keyboardText);
    }

    void processGuess(char guess) {
        if (isLetterGuessed(guess)) {
            messageText.setString("You've already guessed this letter!");
            return;
        }

        guessedLetters[guessedCount++] = guess;
        bool found = false;

        for (int i = 0; i < wordLength; i++) {
            if (word[i] == guess) {
                hiddenWord[i] = guess;
                found = true;
            }
        }
        

        if (found) {
            messageText.setString("Good guess!");
            score += 10;
            soundSystem.playSound("Hangmansound/correct.ogg"); // Play correct sound
        }
        else {
            hangmanFigure.decreaseLives();
            messageText.setString("Wrong guess!");
            score -= 5;
            soundSystem.playSound("Hangmansound/wrong.ogg"); // Play wrong sound
        }
    }

    bool isGameOver() {
        if (hangmanFigure.getLives() <= 0) {
            soundSystem.playSound("Hangmansound/gameover.ogg"); // Play game over sound
            return true;
        }
        
        return false;
    }

    bool isgameguessed() {
        if (isWordGuessed()) {
            //soundSystem.playSound("Hangmansound/win.ogg"); // Play win sound
            return true;
        }
        return false;
    }

    bool isLetterGuessed(char letter) {
        for (int i = 0; i < guessedCount; i++) {
            if (guessedLetters[i] == letter)
                return true;
        }
        return false;
    }

    bool isWordGuessed() {
        for (int i = 0; i < wordLength; i++) {
            if (word[i] != hiddenWord[i])
                return false;
        }
        return true;
    }

    int getScore() const {
        return score;
    }

    string getWord() {
        return string(word);
    }


    void run(RenderWindow& window){
        //Event hangmanEvent;
        soundSystem.musicplay("hangmansound/intro.ogg"); // game sound
        int musicVolume = 50;
        
        while ( window.isOpen()) {

            //adjust VFX volume
               //increase volumn
            if (Keyboard::isKeyPressed(Keyboard::Multiply)) {
                musicVolume = min(musicVolume + 5, 100); // Max 100
                soundSystem.adjustVolume(musicVolume);
                cout << "Music Volume: " << musicVolume << endl;
            }
            // decrease volume
            if (Keyboard::isKeyPressed(Keyboard::Divide)) {
                musicVolume = max(musicVolume - 5, 0); // Min 0
                soundSystem.adjustVolume(musicVolume);
                cout << "Music Volume: " << musicVolume << endl;
            }

            Event hangmanEvent;
            while (window.pollEvent(hangmanEvent)) {
                if (hangmanEvent.type == Event::Closed) {
                    window.close();
                }

                else if (hangmanEvent.type == Event::KeyPressed) {
                    if (hangmanEvent.key.code == inputsystem.isActionActive("Escape")) {
                        
                    }
                    else if (hangmanEvent.key.code == Keyboard::Space) {
                        // Stop the intro music when space is pressed
                        soundSystem.stopMusic();
                    }
                }
                else if (hangmanEvent.type == Event::TextEntered) {
                    char inputChar = inputsystem.textenter(hangmanEvent);
                    if (inputChar >= 'a' && inputChar <= 'z') {
                        processGuess(inputChar);
                    }
                    else if (inputChar >= 'A' && inputChar <= 'Z') {
                        processGuess(inputChar + 32);
                    }
                }





            }

            window.clear(Color::White);
            draw(window);
            window.display();

            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }

            if (isWordGuessed()) {
                soundSystem.playSound("Hangmansound/win.ogg"); 
                messageText.setString("Congratulations! The word was: " + string(word));
                window.clear(Color::White);
                draw(window);
                window.display();
                sleep(seconds(5));  
                resetGame();        
            }

            if (isGameOver()) {
                messageText.setString("OOPS! The word was: " + string(word));
                window.clear(Color::White);
                draw(window);
                window.display();
                sleep(seconds(6));
                //window.close();
                break;
            }
        }
    }
};

