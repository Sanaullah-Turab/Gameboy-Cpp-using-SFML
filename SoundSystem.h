#pragma once
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include"Game.h"
using namespace std;


class SoundSystem {
private:
    sf::SoundBuffer soundBuffer;
    sf::Sound sound;
    sf::Music music;

public:
    // Plays a sound from a file
    bool playSound(const string& soundFile) {
        if (!soundBuffer.loadFromFile(soundFile)) {
            cout << "Error: Could not load sound file: " << soundFile << endl;
            return false;
        }
        sound.setBuffer(soundBuffer);
        sound.play();
        return true;
    }

    bool musicplay(const string& soundFile) {
        if (!music.openFromFile(soundFile)) {
            cout << "Error: Could not load the file " << soundFile << endl;
            return false;
        }
        music.play();
        return true;
    }

    // Stops the current sound
    void stopMusic() {
        music.stop();
    }

    void stopSound() {
        sound.stop();
    }

    // Adjusts the volume (0 to 100)
    void adjustVolume(int level) {
        if (level < 0) level = 0;
        if (level > 100) level = 100;
        sound.setVolume(static_cast<float>(level));
    }

    void adjustmusicVolume(int level) {
        if (level < 0) level = 0;
        if (level > 100) level = 100;
        music.setVolume(static_cast<float>(level));
    }
};
