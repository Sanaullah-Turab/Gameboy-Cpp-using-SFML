#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
using namespace sf;
using namespace std;

class InputState {
public:
    bool isPressed = false;
    string action = "";
};

class InputSystem {
private:
    InputState keyMappings[Keyboard::KeyCount];
    bool previousKeyStates[Keyboard::KeyCount];

public:
    InputSystem() {
        for (int i = 0; i < Keyboard::KeyCount; ++i) {
            keyMappings[i].isPressed = false;
            keyMappings[i].action = "";
            previousKeyStates[i] = false;
        }
    }

    char textenter(Event& event) {
        if (event.type == Event::TextEntered) {
            char temp = static_cast<char>(event.text.unicode);
            return temp;
        }
    }

    void handleInput() {
        for (int i = 0; i < Keyboard::KeyCount; ++i) {
            bool isCurrentlyPressed = Keyboard::isKeyPressed(static_cast<Keyboard::Key>(i));
            keyMappings[i].isPressed = isCurrentlyPressed && !previousKeyStates[i]; // Detect key press
            previousKeyStates[i] = isCurrentlyPressed; // Update previous state
        }
    }

    void mapKey(Keyboard::Key key, const string& action) {
        keyMappings[key].action = action;
    }

    bool isActionActive(const string& action) {
        for (int i = 0; i < Keyboard::KeyCount; ++i) {
            if (keyMappings[i].isPressed && keyMappings[i].action == action) {
                return true;
            }
        }
        return false;
    }
};
