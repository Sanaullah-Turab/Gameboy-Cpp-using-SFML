#pragma once

#include <SFML/Graphics.hpp>
#include <cstdlib> 
#include <ctime>
#include "Game.h"

using namespace std;
using namespace sf;



const int TILE_SIZE = 35;
const int GRID_WIDTH = 34;
const int GRID_HEIGHT = 23;
const int MAX_SEGMENTS = 100; // max length of the snake


class Point {
public:
    int x, y;
    Point(int px = 0, int py = 0) : x(px), y(py) {}
};


class Grid {
public:
    int tiles[GRID_WIDTH][GRID_HEIGHT];

    Grid() {
        // Initialize the grid with empty spaces
        for (int i = 0; i < GRID_WIDTH; ++i) {
            for (int j = 0; j < GRID_HEIGHT; ++j) {
                tiles[i][j] = 0;
            }
        }
    }

    //to generate grid of the game
    void generate() {
        for (int i = 0; i < GRID_WIDTH; ++i) {
            for (int j = 0; j < GRID_HEIGHT; ++j) {
                if (i == 0 || i == GRID_WIDTH - 1 || j == 0 || j == GRID_HEIGHT - 1) {
                    tiles[i][j] = 1; // wall
                }
                else {
                    tiles[i][j] = 0; // empty space
                }
            }
        }
    }
};

class Snake {
public:
    Point segments[MAX_SEGMENTS];
    int length;
    int direction;
    int growCount;

    Snake() {
        init(10, 10, 1, 4); //postion of the snake for the start
    }

    //this is for the point where game start and snake is set at its intital starting point and direction
    void init(int startX, int startY, int startDirection, int startLength) {
        length = startLength;
        direction = startDirection;
        growCount = 0;

        // Set initial body segments
        for (int i = 0; i < length; ++i) {
            segments[i] = Point(startX - i, startY);
        }
    }

    // increaes growth
    void grow() {
        growCount++;
    }

    Point nextMove() {
        int dx = 0, dy = 0;
        if (direction == 0)
            dy = -1; // Moving up
        else if (direction == 1)
            dx = 1; // Moving right
        else if (direction == 2)
            dy = 1; // Moving down
        else if (direction == 3)
            dx = -1; // Moving left

        return Point(segments[0].x + dx, segments[0].y + dy);
    }

    void move() {
        Point nextPos = nextMove();

        // for body to follow head
        for (int i = length - 1; i > 0; --i) {
            segments[i] = segments[i - 1];
        }
        segments[0] = nextPos;

        // bonus??easter
        if (growCount > 0 && length < MAX_SEGMENTS) {
            segments[length] = segments[length - 1];
            length++;
            growCount--;
        }
    }
};

//inhertitance with the game
class SnakeGame :public Game {
private:
    RenderWindow window; // Game window
    Texture texture, t1;
    Sprite sprite;
    Sprite s1;
    Grid grid;
    Snake snake;
    Point food;
    bool gameOver;
    int score;
    Font font;
    Text scoreText;
    Text playerNameText;
    //Game game;

public:
    SnakeGame() : Game() {
        texture.loadFromFile("snakegamepic/snake-graphics_11zon.jpg");
        sprite.setTexture(texture);
        gameOver = false;
        score = 0;
        grid.generate(); // make grid with walls
        placeFood(); // place the start food
        inputsystem.mapKey(Keyboard::Up, "Up");
        inputsystem.mapKey(Keyboard::Down, "Down");
        inputsystem.mapKey(Keyboard::Left, "Left");
        inputsystem.mapKey(Keyboard::Right, "Right");
        inputsystem.mapKey(Keyboard::Space, "Space");

        if (!font.loadFromFile("Fonts/arial/arial.ttf")) {
            cerr << "Error loading font" << endl;
        }

        // Initialize score text
        scoreText.setFont(font);
        scoreText.setCharacterSize(30);
        scoreText.setFillColor(Color::White);
        scoreText.setString("Score: 0");


        playerNameText.setFont(font);
        playerNameText.setCharacterSize(30);
        playerNameText.setFillColor(Color::White);
        playerNameText.setPosition(10, 50);
    }



    //place the food randomnly to generate with in the grid
    void placeFood() {
        do {
            food.x = rand() % GRID_WIDTH;
            food.y = rand() % GRID_HEIGHT;
        } while (grid.tiles[food.x][food.y] != 0 || isSnakeAt(food.x, food.y));
    }

    //check if the place is occupied 
    bool isSnakeAt(int x, int y) {
        for (int i = 0; i < snake.length; ++i) {
            if (snake.segments[i].x == x && snake.segments[i].y == y) {
                return true;
            }
        }
        return false;
    }

    //for update the game
    void update() {
        if (gameOver)
            return;

        Point nextPos = snake.nextMove();


        //check if the snake collied with grid
        if (nextPos.x < 0 || nextPos.x >= GRID_WIDTH || nextPos.y < 0 || nextPos.y >= GRID_HEIGHT || grid.tiles[nextPos.x][nextPos.y] == 1) {
            gameOver = true;
            soundSystem.playSound("snakegamesound/music/gameover.ogg"); // Gameover sound
            return;
        }

        //check if the sanke colliedes with its own body
        for (int i = 0; i < snake.length; ++i) {
            if (snake.segments[i].x == nextPos.x && snake.segments[i].y == nextPos.y) {
                gameOver = true;
                soundSystem.playSound("snakegamesound/music/gameover.ogg"); // Gameover sound
                return;
            }
        }

        snake.move();




        // for checking if the snkae has eaten the foos
        if (nextPos.x == food.x && nextPos.y == food.y) {
            snake.grow();
            soundSystem.playSound("snakegamesound/music/food.ogg"); //food eat sound
            score++;
            placeFood();
        }
    }
    //for draw
    void draw(RenderWindow& window) {
        window.clear();
        window.draw(s1);


        // grid draw (walls)
        for (int i = 0; i < GRID_WIDTH; ++i) {
            for (int j = 0; j < GRID_HEIGHT; ++j) {
                if (grid.tiles[i][j] == 1) {
                    sprite.setTextureRect(IntRect(0, 0, TILE_SIZE, TILE_SIZE)); // Wall sprite
                    sprite.setPosition(i * TILE_SIZE, j * TILE_SIZE);
                    window.draw(sprite);
                }
            }
        }

        //  food draw
        sprite.setTextureRect(IntRect(0, 120, 32, 32)); // Food sprite
        sprite.setPosition(food.x * TILE_SIZE, food.y * TILE_SIZE);
        window.draw(sprite);

        //show score
        scoreText.setString("Score: " + to_string(score));


        //  snake draw
        for (int i = 0; i < snake.length; ++i) {
            if (i == 0) {
                //  head draw
                IntRect headRect;
                switch (snake.direction) {
                case 0:
                    headRect = IntRect(110, 0, TILE_SIZE, TILE_SIZE);
                    break; // up
                case 1:
                    headRect = IntRect(149, 0, 39, 39);
                    break; // right
                case 2:
                    headRect = IntRect(147, 40, TILE_SIZE, TILE_SIZE);
                    break; // down
                case 3:
                    headRect = IntRect(110, 40, 39, 39);
                    break; // keft
                }
                sprite.setTextureRect(headRect);
            }
            else if (i == snake.length - 1) {
                // tail
                Point diff = Point(snake.segments[i - 1].x - snake.segments[i].x, snake.segments[i - 1].y - snake.segments[i].y);
                IntRect tailRect;
                if (diff.x > 0)
                    tailRect = IntRect(148, 80, TILE_SIZE, TILE_SIZE); // Rright
                else if
                    (diff.x < 0)
                    tailRect = IntRect(110, 120, TILE_SIZE, TILE_SIZE); // left
                else if
                    (diff.y > 0)
                    tailRect = IntRect(148, 120, TILE_SIZE, TILE_SIZE); // Ddown
                else
                    tailRect = IntRect(110, 80, TILE_SIZE, TILE_SIZE); // up
                sprite.setTextureRect(tailRect);
            }
            else {
                // draw body parts
                Point prevDiff = Point(snake.segments[i - 1].x - snake.segments[i].x, snake.segments[i - 1].y - snake.segments[i].y);
                Point nextDiff = Point(snake.segments[i].x - snake.segments[i + 1].x, snake.segments[i].y - snake.segments[i + 1].y);

                IntRect bodyRect;
                if (prevDiff.x != 0 && nextDiff.x != 0)
                    bodyRect = IntRect(42, 0, TILE_SIZE, TILE_SIZE); // up down
                else if (prevDiff.y != 0 && nextDiff.y != 0)
                    bodyRect = IntRect(72, 40, TILE_SIZE, TILE_SIZE); // left right
                // Handle corners
                if (prevDiff.x > 0 && nextDiff.y > 0) {
                    // Bottom-Right Corner
                    bodyRect = IntRect(0, 40, TILE_SIZE, TILE_SIZE);
                }
                else if (nextDiff.x > 0 && prevDiff.y > 0) {
                    bodyRect = IntRect(72, 0, TILE_SIZE, TILE_SIZE);
                }

                //done
                else if (prevDiff.x < 0 && nextDiff.y > 0) {
                    // Bottom-Left Corner
                    bodyRect = IntRect(74, 80, TILE_SIZE, TILE_SIZE);

                }
                else if (nextDiff.x < 0 && prevDiff.y > 0) {
                    bodyRect = IntRect(0, 0, TILE_SIZE, TILE_SIZE);

                }
                //done
                else if (prevDiff.x > 0 && nextDiff.y < 0) {
                    // right up Corner
                    bodyRect = IntRect(0, 0, TILE_SIZE, TILE_SIZE);

                }
                //done
                else if (nextDiff.x > 0 && prevDiff.y < 0) {
                    //left up corner
                    bodyRect = IntRect(74, 80, TILE_SIZE, TILE_SIZE);
                }

                //done
                else if (prevDiff.x < 0 && nextDiff.y < 0) {
                    // Left corner
                    bodyRect = IntRect(72, 0, TILE_SIZE, TILE_SIZE);
                }
                //done
                else if (nextDiff.x < 0 && prevDiff.y < 0) {
                    //right corner
                    bodyRect = IntRect(0, 40, TILE_SIZE, TILE_SIZE);
                }
                sprite.setTextureRect(bodyRect);
            }
            sprite.setPosition(snake.segments[i].x * TILE_SIZE, snake.segments[i].y * TILE_SIZE);
            window.draw(sprite);
        }
        window.draw(scoreText);
        window.display();
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

    }

    // game loop
    void run(RenderWindow& window) {
        //if (!isNameEntered) {
        //    inputPlayerName(window);
        //    cout << "Player name entered: " << playerName << endl;  // Debug output
        //}
        Clock clock;
        float moveTimer = 0.0f;
        const float moveInterval = 0.15f; // snake movement speed
        soundSystem.musicplay("snakegamesound/music/music.ogg"); // game sound
        int musicVolume = 50;
        soundSystem.adjustmusicVolume(musicVolume);
        if (!t1.loadFromFile("snakegamepic/background_11zon.jpg")) {
            cout << "Error loading texture" << endl;
            return;
        }
        s1.setTexture(t1);

        while (window.isOpen()) {
            Event event;
            while (window.pollEvent(event)) {
                if (event.type == Event::Closed) {
                    window.close();
                }
                inputsystem.handleInput();
                //direction change
                if (inputsystem.isActionActive("Up") && snake.direction != 2) {
                    snake.direction = 0;
                    soundSystem.playSound("snakegamesound/music/move.ogg"); // moement sound
                }
                else if (inputsystem.isActionActive("Right") && snake.direction != 3) {
                    snake.direction = 1;
                    soundSystem.playSound("snakegamesound/music/move.ogg"); // moement sound
                }
                else if (inputsystem.isActionActive("Down") && snake.direction != 0) {
                    snake.direction = 2;
                    soundSystem.playSound("snakegamesound/music/move.ogg"); // moement sound
                }
                else if (inputsystem.isActionActive("Left") && snake.direction != 1) {
                    snake.direction = 3;
                    soundSystem.playSound("snakegamesound/music/move.ogg"); // moement sound
                }
                else if (inputsystem.isActionActive("Space")) {
                    snake.grow();
                }

                //adjust music volume
               //increase volumn
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

            }

            // update game state based on timer
            float deltaTime = clock.restart().asSeconds();
            moveTimer += deltaTime;

            if (moveTimer >= moveInterval) {
                update();
                moveTimer = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                break;
            }
            if (gameOver) {
                updateLeaderboard(playerName, score);
                cout << "Saving score: " << playerName << " - " << score << endl;
                sleep(seconds(1));
                break;
            }

            if (Keyboard::isKeyPressed(Keyboard::P)) {
                while (true) {

                    drawEndMessage(window, "\tGame Paused!\n\n\n Lctrl to continue", 1);
                    if (Keyboard::isKeyPressed(Keyboard::LControl)) {
                        break;
                    }
                }

            }

            draw(window); // render the game elements
        }

    }
    int getScore() const {
        return score;
    }
};