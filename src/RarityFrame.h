//
// Created by lazab on 12/10/2025.
//
/*
#ifndef SFMLPROJECT_RARITYFRAME_H
#define SFMLPROJECT_RARITYFRAME_H
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>

enum class Rarity { Common, Rare, Epic, Legendary, Hero };

class RarityFrame {
private:
    sf::Texture texture;
    sf::Sprite sprite;

public:
    RarityFrame(Rarity rarity) {
        std::string path;
        switch (rarity) {
            case Rarity::Common:    path = "assets/cards/frame_common.png"; break;
            case Rarity::Rare:      path = "assets/cards/frame_rare.png"; break;
            case Rarity::Epic:      path = "assets/cards/frame_epic.png"; break;
            case Rarity::Legendary: path = "assets/cards/frame_legendary.png"; break;
            case Rarity::Hero:      path = "assets/cards/frame_hero.png"; break;
        }

        if (!texture.loadFromFile(path)) {
            std::cerr << "Greska pri ucitavanju rarity frame-a: " << path << std::endl;
        }

        sprite.setTexture(texture);
    }
};

#endif //SFMLPROJECT_RARITYFRAME_H*/