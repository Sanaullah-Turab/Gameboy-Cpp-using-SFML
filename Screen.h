#pragma once
#include <SFML/Graphics.hpp>
#include "MainMenu.h"
#include "Game.h"
#include "transition.h"

using namespace sf;

class Screen {
private:
    int resolutionX;
    int resolutionY;
    const unsigned int FPS = 60;

public:
    RenderWindow window;
    Screen(): window(VideoMode(1200, 800), "Game Menu System") {
        resolutionX = 1200;
        resolutionY = 800;
        window.setFramerateLimit(FPS);
    }

};