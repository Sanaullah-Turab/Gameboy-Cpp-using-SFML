#include <SFML/Graphics.hpp>
#include "Snakegame.h"

using namespace sf;

int main()
{
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation
    SnakeGame game;
    game.run(); // Start the game
    return 0;
}
