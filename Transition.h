#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace sf;

bool showNintendoTransition(RenderWindow& window) {
    //declaring textre of the picture
    Texture ninten;
    if (!ninten.loadFromFile("gameboypic/nintendo.jpg")) {
        cout << "Error in loading picutre " << endl;
        return false;
    }
    Sprite sprite(ninten);
    sprite.setTextureRect(IntRect(0, 0, 134, 65));

    // giving x and y axis where picture should apper
    float xSpritePos = (window.getSize().x - sprite.getLocalBounds().width) / 2;
    float ySpritePos = (window.getSize().y - sprite.getLocalBounds().height) / 2;
    sprite.setPosition(xSpritePos, ySpritePos);

    //using clock for the transition
    Clock clk;
    float fadeDuration = 1.0f;
    float displayTime = 2.0f;
    float fadeOutDuration = 1.0f;
    float totalDuration = fadeDuration + displayTime + fadeOutDuration;
    // Declare a new sound buffer
    SoundBuffer buffer;

    // Load it from a file
    if (!buffer.loadFromFile("Gameboysound/nintendosound.ogg"))
    {
        cout << "sound not working" << endl;
    }

    // Create a sound source and bind it to the buffer
    Sound sound1;
    sound1.setBuffer(buffer);

    // Play the sound
    sound1.play();

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
                return false;
            }
        }

        float elapsed = clk.getElapsedTime().asSeconds();


        float alpha = 0;
        if (elapsed < fadeDuration) {

            alpha = (elapsed / fadeDuration) * 255;
        }
        else if (elapsed < fadeDuration + displayTime) {

            alpha = 255;
        }
        else if (elapsed < totalDuration) {

            float fadeOutProgress = (elapsed - (fadeDuration + displayTime)) / fadeOutDuration;
            alpha = 255 * (1 - fadeOutProgress);
        }
        else {

            return true;
        }

        // text.setFillColor(Color(255, 255, 255, static_cast<Uint8>(alpha)));
        sprite.setColor(Color(255, 255, 255, static_cast<Uint8>(alpha)));

        window.clear(Color::Black);
        window.draw(sprite);
        // window.draw(text);
        window.display();
    }

    return false;
}