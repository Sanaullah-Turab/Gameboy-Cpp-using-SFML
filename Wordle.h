#pragma once
#include<iostream>
#include<cstdlib>
#include<ctime>
#include"Game.h"
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;


class Dictionary {
	const char wordStorage[100][6] = {
		"apple", "brave", "crane", "flame", "grape", "house", "jolly", "knife", "lemon", "mango",
		"ninja", "olive", "peach", "quilt", "robot", "sheep", "table", "urban", "vowel", "whale",
		"xenon", "yacht", "zebra", "alert", "blush", "candy", "daisy", "eager", "fancy", "glint",
		"hasty", "index", "jumpy", "koala", "laser", "magic", "noble", "ocean", "piano", "quest",
		"river", "sword", "tiger", "unity", "vivid", "waste", "yummy", "angle", "bloom", "cloud",
		"dream", "eagle", "frost", "globe", "heart", "ivory", "jewel", "knack", "loyal", "mirth",
		"nerdy", "oasis", "plumb", "quirk", "ready", "sassy", "tough", "upset", "vague", "witty",
		"xylos", "zesty", "beard", "charm", "dance", "earth", "faith", "glide", "hills", "icing",
		"jokes", "kings", "light", "miles", "night", "opera", "peace", "quiet", "rocks", "smile",
		"train", "winds", "glory"
	};

public:
	void randomSelect(string& targetWord) {

		int r = rand() % 100;
		targetWord = wordStorage[r];
		cout << r << endl;

		cout << "Target word: " << targetWord << endl;

	}
	bool isValidWord(const string& word) {
		string lowerInput = word;
		for (char& ch : lowerInput) {
			ch = tolower(ch);
		}

		for (int i = 0; i < 100; i++) {
			string lowerWord = wordStorage[i];
			for (char& ch : lowerWord) {
				ch = tolower(ch);
			}
			if (lowerInput == lowerWord) {
				return true;
			}
		}
		return false;
	}
};

class KeyBoard {
public:
	string keyboardLayout = "QWERTYUIOPASDFGHJKLZXCVBNM";
	RectangleShape enterKey, backspaceKey;
	Text enterText, backspaceText;
	Font font;
	RectangleShape keyboardKeys[26];
	Text keyboardTexts[26];

	void initializeKeyboard(RenderWindow& window) {
		if (!font.loadFromFile("Fonts/arial/arial.ttf")) {
			cout << "Error loading font" << endl;
			return;
		}
		const float keyWidth = 40.f;
		const float keyHeight = 40.f;
		const float paddingk = 8.f;
		const float startx = (window.getSize().x - 10 * (keyWidth + paddingk)) / 2;
		const float starty = window.getSize().y - 180;

		int currentIndex = 0;

		for (char key : keyboardLayout) {
			RectangleShape keyShape(Vector2f(keyWidth, keyHeight));
			keyShape.setFillColor(Color::White);
			keyShape.setOutlineThickness(2.f);
			keyShape.setOutlineColor(Color::Black);

			float xPos, yPos;

			if (currentIndex < 10) {
				// First row
				xPos = startx + currentIndex * (keyWidth + paddingk);
				yPos = starty;
			}
			else if (currentIndex < 19) {
				// Second row
				xPos = startx + (keyWidth / 2) + (currentIndex - 10) * (keyWidth + paddingk);
				yPos = starty + keyHeight + paddingk;
			}
			else {
				// Third row
				xPos = startx + (keyWidth * 2.f) + (currentIndex - 19) * (keyWidth + paddingk);
				yPos = starty + 2 * (keyHeight + paddingk);
			}

			keyShape.setPosition(xPos, yPos);
			keyboardKeys[currentIndex] = keyShape;

			//  key text
			Text keyText;
			keyText.setFont(font);
			keyText.setString(string(1, key));
			keyText.setCharacterSize(18);
			keyText.setFillColor(Color::Black);
			keyText.setPosition(xPos + keyWidth / 4, yPos + keyHeight / 4);
			keyboardTexts[currentIndex] = keyText;

			currentIndex++;
		}

		// Position Backspace key separate
		backspaceKey.setSize(Vector2f(100.f, keyHeight));
		backspaceKey.setFillColor(Color::White);
		backspaceKey.setOutlineThickness(2.f);
		backspaceKey.setOutlineColor(Color::Black);
		backspaceKey.setPosition(startx, starty + 3 * (keyHeight + paddingk));

		backspaceText.setFont(font);
		backspaceText.setString("Backspace");
		backspaceText.setCharacterSize(14);
		backspaceText.setFillColor(Color::Black);
		backspaceText.setPosition(
			backspaceKey.getPosition().x + backspaceKey.getSize().x / 8,
			backspaceKey.getPosition().y + backspaceKey.getSize().y / 4
		);

		// Position Enter key separate
		enterKey.setSize(Vector2f(100.f, keyHeight));
		enterKey.setFillColor(Color::White);
		enterKey.setOutlineThickness(2.f);
		enterKey.setOutlineColor(Color::Black);
		enterKey.setPosition(
			startx + (keyWidth * 8.f) + 2 * paddingk,
			starty + 3 * (keyHeight + paddingk)
		);

		enterText.setFont(font);
		enterText.setString("Enter");
		enterText.setCharacterSize(14);
		enterText.setFillColor(Color::Black);
		enterText.setPosition(
			enterKey.getPosition().x + enterKey.getSize().x / 4,
			enterKey.getPosition().y + enterKey.getSize().y / 4
		);
	}

};




class WordleGame : public Game {
private:
	string targetWord;
	Dictionary dictionary;
	KeyBoard keyBoard;
	bool isGameEnd;

	Texture t1;
	Sprite s1;

	const int Letters;
	const int Words;
	int score;
	Font font;
	RectangleShape** grid;
	Text** gridText;
	string currentInput;
	int currentRow;
	int currentCol;
	bool isGameOver;
	Text scoreText;

	Clock messageTimer;





public:
	WordleGame() :Letters(5), Words(6) , Game(){

		srand(time(0));
		targetWord = "";
		score = 0;
		currentRow = 0;
		currentCol = 0;
		isGameOver = 0;
		isGameEnd = 0;
		grid = new RectangleShape * [Words];
		gridText = new Text * [Words];
		for (int i = 0; i < Words; i++) {
			grid[i] = new RectangleShape[Letters];
			gridText[i] = new Text[Letters];
		}


	}


	void startGame(RenderWindow& window) {
		initializeScreen(window);
		gameLoop(window);
	}


	void initializeScreen(RenderWindow& window) {
		if (!font.loadFromFile("Fonts/arial/arial.ttf")) {
			cout << "Error loading font" << endl;
			return;
		}
		dictionary.randomSelect(targetWord);

		// Score
		scoreText.setFont(font);
		scoreText.setCharacterSize(30);
		scoreText.setFillColor(Color::White);
		scoreText.setPosition(10, 10);
		scoreText.setString("Score: 0");



		// Grid
		const float boxSize = 50.f;
		const float padding = 8.f;
		const float startX = (window.getSize().x - (Letters * boxSize + (Letters - 1) * padding)) / 2;
		const float startY = 50.f;



		for (int i = 0; i < Words; i++) {
			for (int j = 0; j < Letters; j++) {
				// Setting Boxes
				grid[i][j].setSize(Vector2f(boxSize, boxSize));
				grid[i][j].setFillColor(Color::White);
				grid[i][j].setOutlineThickness(2.f);
				grid[i][j].setOutlineColor(Color::Black);
				grid[i][j].setPosition(startX + j * (boxSize + padding), startY + i * (boxSize + padding));

				// Setting Text
				gridText[i][j].setFont(font);
				gridText[i][j].setCharacterSize(18);
				gridText[i][j].setFillColor(Color::Black);
				gridText[i][j].setPosition(grid[i][j].getPosition().x + 10, grid[i][j].getPosition().y + 5);
			}
		}
		keyBoard.initializeKeyboard(window);

	}



	void gameLoop(RenderWindow& window) {

		soundSystem.musicplay("Wordlesound/backsound.ogg");
		int musicVolume = 50;


		while (window.isOpen()) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) {
					window.close();
				}
				handleInput(event, window);
			}
			scoreText.setString("Score: " + to_string(score));//update score


			window.clear();
			if (Keyboard::isKeyPressed(Keyboard::Add)) {
				musicVolume = min(musicVolume + 5, 100); // Max 100
				soundSystem.adjustmusicVolume(musicVolume);
				cout << "Music Volume: " << musicVolume << endl;
			}
			// decrease volume
			if (Keyboard::isKeyPressed(Keyboard::Subtract)) {
				musicVolume = max(musicVolume - 5, 0); // Min 0
				soundSystem.adjustmusicVolume(musicVolume);
				cout << "Music Volume: " << musicVolume << endl;
			}

			if (Keyboard::isKeyPressed(Keyboard::Escape)) {
				break;
			}
			if (isGameEnd) {
				drawEndMessage(window, "GameOver! Final Score: " + to_string(score), 2);
				cout << "Saving score: " << playerName << " - " << score << endl;  
				updateLeaderboard(playerName, score);

				break;
			}
			if (isGameOver) {
				drawEndMessage(window, "Great! Score: " + to_string(score), 2);
				cout << "Saving score: " << playerName << " - " << score << endl;  
				resetGridText();
				isGameOver = 0;

			}

			drawGrid(window);

			window.display();
		}
	}
	void resetGridText() {
		// Clear text 
		for (int i = 0; i < Words; i++) {
			for (int j = 0; j < Letters; j++) {
				gridText[i][j].setString("");
				grid[i][j].setFillColor(Color::White);
			}
		}
		Color keyColor = Color::White;
		for (int j = 0; j < sizeof(keyBoard.keyboardKeys) / sizeof(keyBoard.keyboardKeys[0]); j++) {

			keyBoard.keyboardKeys[j].setFillColor(keyColor);

		}
		dictionary.randomSelect(targetWord);
		currentInput = "";
		currentRow = 0;
		currentCol = 0;
	}



	// Draw grid, write text and write score
	void drawGrid(RenderWindow& window) {
		for (int i = 0; i < Words; i++) {
			for (int j = 0; j < Letters; j++) {
				window.draw(grid[i][j]);
				window.draw(gridText[i][j]);
			}
		}
		window.draw(scoreText);
		for (int i = 0; i < sizeof(keyBoard.keyboardKeys) / sizeof(keyBoard.keyboardKeys[0]); i++) {
			window.draw(keyBoard.keyboardKeys[i]);
			window.draw(keyBoard.keyboardTexts[i]);
		}

		window.draw(keyBoard.enterKey);
		window.draw(keyBoard.enterText);
		window.draw(keyBoard.backspaceKey);
		window.draw(keyBoard.backspaceText);
	}

	void handleInput(Event event, RenderWindow& window) {

		if (isGameOver) {
			return;
		}

		if (event.type == Event::TextEntered) {
			if (Keyboard::isKeyPressed(Keyboard::Space)) {
				while (true) {

					drawEndMessage(window, "\tGame Paused!\n\n\n Lctrl to continue", 1);
					if (Keyboard::isKeyPressed(Keyboard::LControl)) {
						break;
					}
				}

			}
			char typedChar = static_cast<char>(event.text.unicode);

			if (isalpha(typedChar) && currentCol < Letters) {
				currentInput += toupper(typedChar);
				gridText[currentRow][currentCol].setString(string(1, toupper(typedChar)));
				currentCol++;
			}
		}

		if (event.type == Event::KeyPressed) {
			if (event.key.code == Keyboard::Backspace && !currentInput.empty() && currentCol > 0) {
				currentCol--;
				currentInput.pop_back();
				gridText[currentRow][currentCol].setString("");
			}

			if (event.key.code == Keyboard::Enter) {

				if (dictionary.isValidWord(currentInput)) {
					updateGridColors();
					for (int i = 0; i < 5; i++) {
						targetWord[i] = toupper(targetWord[i]);
					}

					if (targetWord == currentInput) {
						isGameOver = true;
						score += 5;
					}
					else {
						currentRow++;
						currentCol = 0;
						currentInput = "";

						if (currentRow == Words) {
							isGameEnd = 1;
							cout << "Game Over!" << endl;
						}
					}
				}
				else {
					cout << "Invalid word!" << endl;
					drawEndMessage(window, "Invalid word!", 1);
				}
			}
		}

		if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
			Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
			for (int i = 0; i < sizeof(keyBoard.keyboardKeys) / sizeof(keyBoard.keyboardKeys[0]); i++) {
				if (keyBoard.keyboardKeys[i].getGlobalBounds().contains(mousePos)) {
					char clickedKey = keyBoard.keyboardLayout[i];
					if (isalpha(clickedKey) && currentCol < Letters) {
						currentInput += clickedKey;
						gridText[currentRow][currentCol].setString(string(1, clickedKey));
						currentCol++;
					}
				}
			}

			if (keyBoard.enterKey.getGlobalBounds().contains(mousePos)) {
				if (dictionary.isValidWord(currentInput)) {
					updateGridColors();
					for (int i = 0; i < 5; i++) {
						targetWord[i] = toupper(targetWord[i]);
					}

					if (targetWord == currentInput) {
						isGameOver = true;
						score += 5;
					}
					else {
						currentRow++;
						currentCol = 0;
						currentInput = "";

						if (currentRow == Words) {
							cout << "Game Over!" << endl;
						}
					}
				}
				else {
					cout << "Invalid word!" << endl;
					drawEndMessage(window, "Invalid word!", 5);
				}
			}
		}
	}
	void updateKeyColors() {
		for (int i = 0; i < Letters; i++) {
			char letter = currentInput[i];
			Color keyColor = Color::White;

			// Check if the letter is in the target word
			if (targetWord[i] == letter) {
				keyColor = Color::Green;  // Correct position
			}
			else if (targetWord.find(letter) != string::npos) {
				keyColor = Color::Yellow;  // Correct letter, wrong position
			}
			else {
				Color gray(128, 128, 128);
				keyColor = gray;    // Letter not in word
			}

			// Update key color
			for (int j = 0; j < sizeof(keyBoard.keyboardKeys) / sizeof(keyBoard.keyboardKeys[0]); j++) {
				if (keyBoard.keyboardTexts[j].getString() == string(1, letter)) {
					keyBoard.keyboardKeys[j].setFillColor(keyColor);
					break;
				}
			}
		}
	}






	void updateGridColors() {
		int x = 0;
		while (targetWord[x] != '\0') {
			targetWord[x] = toupper(targetWord[x]);
			x++;
		}

		for (int i = 0; i < Letters; i++) {

			if (currentInput[i] == targetWord[i]) {
				grid[currentRow][i].setFillColor(Color::Green); // Correct position and letter
				score += 2;
			}
			else if (targetWord.find(currentInput[i]) != string::npos) {
				grid[currentRow][i].setFillColor(Color::Yellow); // Correct letter, wrong position
				score += 1;
			}
			else {
				Color gray(128, 128, 128);
				grid[currentRow][i].setFillColor(gray); // Incorrect letter
				score -= 1;
			}
		}
		updateKeyColors();

	}
	void drawEndMessage(RenderWindow& window, const string& message, int delaySeconds) {

		Text endMessage;
		endMessage.setFont(font);
		endMessage.setString(message);
		endMessage.setCharacterSize(36);
		endMessage.setFillColor(Color::Red);
		endMessage.setPosition(
			(window.getSize().x - endMessage.getLocalBounds().width) / 2,
			window.getSize().y / 2);

		Clock delayClock;
		while (delayClock.getElapsedTime().asSeconds() < delaySeconds) {
			window.clear();
			window.draw(endMessage);
			window.display();
		}
	}



	int getScore() const {
		return score;
	}

	~WordleGame() {


		for (int i = 0; i < Words; i++) {
			delete[] grid[i];
			delete[] gridText[i];
		}
		delete[] grid;
		delete[] gridText;

	}
};