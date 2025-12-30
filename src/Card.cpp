#include "Card.h"
#include <iostream>
#include <map>

#include "NeutralTexture.h"



Card::Card(std::string name, sf::Texture &tex, int hp, int dmg, int cost,
           Rarity rarity, AttackType attackType,
           const std::string &description) : sprite(getNeutralTexture()), rarityFrameSprite(getNeutralTexture())
{

    // postavljanje vrednosti
    this->name = name;
    this->hp = hp;
    this->dmg = dmg;
    this->cost = cost;
    this->rarity = rarity;
    this->attackType = attackType;
    this->description = description;
    this->faceUp = true;

    // glavni art
    this->texture = &tex;
    //this->texture = std::make_shared<sf::Texture>(tex);
    this->sprite = sf::Sprite(tex); // konstrukt sa teksturom odmah

    // putanja rarity frame-a
    /*std::string framePath;
    switch (rarity) {
        case Rarity::Common: framePath = "assets/frames/frame_common.png";
            break;
        case Rarity::Rare: framePath = "assets/frames/frame_rare.png";
            break;
        case Rarity::Epic: framePath = "assets/frames/frame_epic.png";
            break;
        case Rarity::Legendary: framePath = "assets/frames/frame_legendary.png";
            break;
        case Rarity::Hero: framePath = "assets/frames/frame_hero.png";
            break;
    }*/



    // sprite sa teksturom
    rarityFrameSprite.setTexture(getRarityFrame(rarity));

}


// UCITAVANJE GETTER RARITY FRAME TEKSTURE
sf::Texture& Card::getRarityFrame(Rarity r)
{
    static std::map<Rarity, sf::Texture> cache;

    if (!cache.contains(r))
    {
        std::string path;
        switch (r) {
            case Rarity::Common: path = "assets/frames/frame_common.png"; break;
            case Rarity::Rare: path = "assets/frames/frame_rare.png"; break;
            case Rarity::Epic: path = "assets/frames/frame_epic.png"; break;
            case Rarity::Legendary: path = "assets/frames/frame_legendary.png"; break;
            case Rarity::Hero: path = "assets/frames/frame_hero.png"; break;
        }


        if (!cache[r].loadFromFile(path))
        {
            std::cerr << "Ne mogu da ucitam rarity frame: " << path << std::endl;
            cache[r] = getNeutralTexture(); // fallback
        }

    }
    return cache[r];
}







void Card::setBrightness(bool value)
{
    if (value)
        sprite.setColor(sf::Color::White); // puna boja
    else
        sprite.setColor(sf::Color(120, 120, 120)); // blago potamnjeno
}

void Card::resetVisuals()
{
    sprite.setColor(sf::Color::White);
}

