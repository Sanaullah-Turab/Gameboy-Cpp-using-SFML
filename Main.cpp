#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
//#include "MainMenu.h"
#include "hangman.h"
//#include "Transition.h"
#include "Snakegame.h"
#include "Screen.h"
#include "Gameboy.h"

using namespace sf;

int main() {
    Gameboy game;
    game.startGame();
    return 0;
}



#pragma once

// Main includes for SFML library and other utilities
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
//#include "Game.h"
//#include"Screen.h"

using namespace sf;

// Constants for tile size and grid dimensions
const int TILE_SIZE = 35; // Each tile is 32x32 pixels
const int GRID_WIDTH = 20; // Grid is 20 tiles wide
const int GRID_HEIGHT = 15; // Grid is 15 tiles high
const int MAX_SEGMENTS = 100; // Maximum length of the snake

// Class representing a point on the grid
class Point {
public:
   int x, y;
   Point(int px = 0, int py = 0) : x(px), y(py) {}
};

// Class for managing the game grid
class Grid {
public:
   int tiles[GRID_WIDTH][GRID_HEIGHT]; // Grid matrix

   Grid() {
       // Initialize the grid with empty spaces
       for (int i = 0; i < GRID_WIDTH; ++i) {
           for (int j = 0; j < GRID_HEIGHT; ++j) {
               tiles[i][j] = 0;
           }
       }
   }

   // Generate walls around the grid
   void generate() {
       for (int i = 0; i < GRID_WIDTH; ++i) {
           for (int j = 0; j < GRID_HEIGHT; ++j) {
               if (i == 0 || i == GRID_WIDTH - 1 || j == 0 || j == GRID_HEIGHT - 1) {
                   tiles[i][j] = 1; // Wall
               }
               else {
                   tiles[i][j] = 0; // Empty space
               }
           }
       }
   }
};

// Class representing the snake
class Snake {
public:
   Point segments[MAX_SEGMENTS]; // Snake's body segments
   int length; // Current length of the snake
   int direction; // Direction: 0 (Up), 1 (Right), 2 (Down), 3 (Left)
   int growCount; // Number of segments to grow

   Snake() {
       init(10, 10, 1, 4); // Initialize snake with default position and length
   }

   // Initialize snake with position, direction, and length
   void init(int startX, int startY, int startDirection, int startLength) {
       length = startLength;
       direction = startDirection;
       growCount = 0;

       // Set initial body segments
       for (int i = 0; i < length; ++i) {
           segments[i] = Point(startX - i, startY);
       }
   }

   // Increase the snake's length
   void grow() {
       growCount++;
   }

   // Calculate the next position of the snake's head
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

   // Move the snake to the next position
   void move() {
       Point nextPos = nextMove();

       // Shift segments to follow the head
       for (int i = length - 1; i > 0; --i) {
           segments[i] = segments[i - 1];
       }
       segments[0] = nextPos;

       // Handle growth if needed
       if (growCount > 0 && length < MAX_SEGMENTS) {
           segments[length] = segments[length - 1];
           length++;
           growCount--;
       }
   }
};

// Class for managing the Snake Game
class SnakeGame {
private:
   RenderWindow window; // Game window
   Texture texture; // Texture for the sprites
   Sprite sprite; // Sprite for rendering
   Grid grid; // Game grid
   Snake snake; // Snake instance
   Point food; // Food position
   bool gameOver; // Game state
   int score; // Player score

public:
   SnakeGame() : window(VideoMode(GRID_WIDTH* TILE_SIZE, GRID_HEIGHT* TILE_SIZE), "Snake Game") {
       texture.loadFromFile("snakegamepic/snake-graphics_11zon.png");
       sprite.setTexture(texture);
       gameOver = false;
       score = 0;
       grid.generate(); // Generate grid with walls
       placeFood(); // Place the initial food
   }

   // Randomly place food on the grid
   void placeFood() {
       do {
           food.x = rand() % GRID_WIDTH;
           food.y = rand() % GRID_HEIGHT;
       } while (grid.tiles[food.x][food.y] != 0 || isSnakeAt(food.x, food.y));
   }

   // Check if a point is occupied by the snake
   bool isSnakeAt(int x, int y) {
       for (int i = 0; i < snake.length; ++i) {
           if (snake.segments[i].x == x && snake.segments[i].y == y) {
               return true;
           }
       }
       return false;
   }

   // Update game state
   void update() {
       if (gameOver) return;

       Point nextPos = snake.nextMove();

       // Check for collision with walls or itself
       if (nextPos.x < 0 || nextPos.x >= GRID_WIDTH || nextPos.y < 0 || nextPos.y >= GRID_HEIGHT || grid.tiles[nextPos.x][nextPos.y] == 1) {
           gameOver = true;
           return;
       }

       // Check for collision with snake's body
       for (int i = 0; i < snake.length; ++i) {
           if (snake.segments[i].x == nextPos.x && snake.segments[i].y == nextPos.y) {
               gameOver = true;
               return;
           }
       }

       snake.move();

       // Check if snake eats the food
       if (nextPos.x == food.x && nextPos.y == food.y) {
           snake.grow();
           score++;
           placeFood();
       }
   }

   // Draw the game elements
   void draw() {
       window.clear();

       // Draw grid (walls)
       for (int i = 0; i < GRID_WIDTH; ++i) {
           for (int j = 0; j < GRID_HEIGHT; ++j) {
               if (grid.tiles[i][j] == 1) {
                   sprite.setTextureRect(IntRect(0, 0, TILE_SIZE, TILE_SIZE)); // Wall sprite
                   sprite.setPosition(i * TILE_SIZE, j * TILE_SIZE);
                   window.draw(sprite);
               }
           }
       }

       // Draw food
       sprite.setTextureRect(IntRect(0, 120, 32, 32)); // Food sprite
       sprite.setPosition(food.x * TILE_SIZE, food.y * TILE_SIZE);
       window.draw(sprite);

       // Draw snake
       for (int i = 0; i < snake.length; ++i) {
           if (i == 0) {
               // Draw head
               IntRect headRect;
               switch (snake.direction) {
               case 0: 
                   headRect = IntRect(110, 0, TILE_SIZE, TILE_SIZE); 
                   break; // Up
               case 1: 
                   headRect = IntRect(149, 0, 39, 39);
                   break; // Right
               case 2: 
                   headRect = IntRect(147, 40, TILE_SIZE, TILE_SIZE); 
                   break; // Down
               case 3: 
                   headRect = IntRect(110, 40, 39,39 );
                   break; // Left
               }
               sprite.setTextureRect(headRect);
           }
           else if (i == snake.length - 1) {
               // Draw tail
               Point diff = Point(snake.segments[i - 1].x - snake.segments[i].x,snake.segments[i - 1].y - snake.segments[i].y);
               IntRect tailRect;
               if (diff.x > 0) 
                   tailRect = IntRect(148, 80, TILE_SIZE, TILE_SIZE); // Right
               else if
                   (diff.x < 0) 
                   tailRect = IntRect(110, 120, TILE_SIZE, TILE_SIZE); // Left
               else if 
                   (diff.y > 0) 
                   tailRect = IntRect(148, 120, TILE_SIZE, TILE_SIZE); // Down
               else 
                   tailRect = IntRect(110, 80, TILE_SIZE, TILE_SIZE); // Up
               sprite.setTextureRect(tailRect);
           }
           else {
               // Draw body segment
               Point prevDiff = Point(snake.segments[i - 1].x - snake.segments[i].x,snake.segments[i - 1].y - snake.segments[i].y);
               Point nextDiff = Point(snake.segments[i].x - snake.segments[i + 1].x,snake.segments[i].y - snake.segments[i + 1].y);

               IntRect bodyRect;
               if (prevDiff.x != 0 && nextDiff.x != 0)
                   bodyRect = IntRect(72, 0, TILE_SIZE, TILE_SIZE); // Horizontal
               else if (prevDiff.y != 0 && nextDiff.y != 0) 
                   bodyRect = IntRect(72, 40, TILE_SIZE, TILE_SIZE); // Vertical
               else 
                   bodyRect = IntRect(32, 0, TILE_SIZE, TILE_SIZE); // Corner
               sprite.setTextureRect(bodyRect);
           }
           sprite.setPosition(snake.segments[i].x * TILE_SIZE, snake.segments[i].y * TILE_SIZE);
           window.draw(sprite);
       }

       window.display();
   }

   // Main game loop
   void run() {
       Clock clock;
       float moveTimer = 0.0f;
       const float moveInterval = 0.15f; // Interval for snake movement

       while (window.isOpen()) {
           Event event;
           while (window.pollEvent(event)) {
               if (event.type == Event::Closed) {
                   window.close();
               }
               else if (event.type == Event::KeyPressed) {
                   // Change snake direction based on key input
                   if (event.key.code == Keyboard::Up && snake.direction != 2)
                       snake.direction = 0;
                   else if (event.key.code == Keyboard::Right && snake.direction != 3)
                       snake.direction = 1;
                   else if (event.key.code == Keyboard::Down && snake.direction != 0)
                       snake.direction = 2;
                   else if (event.key.code == Keyboard::Left && snake.direction != 1)
                       snake.direction = 3;
               }
           }

           // Update game state based on timer
           float deltaTime = clock.restart().asSeconds();
           moveTimer += deltaTime;

           if (moveTimer >= moveInterval) {
               update();
               moveTimer = 0;
           }

           draw(); // Render the game elements
       }
   }
};

// Entry point of the program
int main() {
   srand(static_cast<unsigned>(time(0))); // Seed for random number generation
   SnakeGame game;
   game.run(); // Start the game
   return 0;
}