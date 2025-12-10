
#include "card.h"
#include <iostream>

Card::Card(sf::Texture &tex, int hp, int dmg, int cost,
           Rarity rarity, AttackType attackType,
           const std::string &description) : sprite(), rarityFrameSprite() {
    // postavljanje vrednosti
    this->hp = hp;
    this->dmg = dmg;
    this->cost = cost;
    this->rarity = rarity;
    this->attackType = attackType;
    this->description = description;
    this->faceUp = true;

    // glavni art
    this->texture = &tex;
    this->sprite = sf::Sprite(tex); // konstrukt sa teksturom odmah

    // putanja rarity frame-a
    std::string framePath;
    switch (rarity) {
        case Rarity::Common: framePath = "assets/cards/frame_common.png";
            break;
        case Rarity::Rare: framePath = "assets/cards/frame_rare.png";
            break;
        case Rarity::Epic: framePath = "assets/cards/frame_epic.png";
            break;
        case Rarity::Legendary: framePath = "assets/cards/frame_legendary.png";
            break;
        case Rarity::Hero: framePath = "assets/cards/frame_hero.png";
            break;
    }

    // učitavanje rarity frame teksture
    if (!rarityFrameTexture.loadFromFile(framePath)) {
        std::cerr << "Greska pri ucitavanju rarity frame-a: " << framePath << std::endl;
    }

    // sprite sa teksturom
    rarityFrameSprite = sf::Sprite(rarityFrameTexture);
}