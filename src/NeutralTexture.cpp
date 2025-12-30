#include "NeutralTexture.h"
#include <iostream>

sf::Texture& getNeutralTexture() {
    static sf::Texture neutralTexture;
    static bool initialized = false;

    if (!initialized) {
        if (!neutralTexture.loadFromFile("assets/neutral.png")) {
            std::cerr << "Greska pri ucitavanju neutralne teksture!" << std::endl;
        }
        initialized = true;
    }

    return neutralTexture;
}



/*
sf::Texture& getNeutralTexture() {
    static sf::Texture tex;
    static bool loaded = false;
    if (!loaded) {
        tex.loadFromMemory(pixel, 4, {1,1});
        loaded = true;
    }
    return tex;
}
*/