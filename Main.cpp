#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
// #include "MainMenu.h"
#include "hangman.h"
// #include "Transition.h"
#include "Snakegame.h"
#include "Screen.h"
#include "Gameboy.h"

using namespace sf;

int main()
{
    Gameboy game;
    game.startGame();
    return 0;
}
