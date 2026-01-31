#include "Card.h"
#include <iostream>
#include <map>

#include "NeutralTexture.h"



Card::Card(std::string name, sf::Texture& tex, int hp, int dmg, int cost,
           Rarity rarity,
           BaseAttack base,
           AttackModifier mods,
           int hits,
           const std::string& description,
           PlacementType placement)
    : sprite(getNeutralTexture()),
      rarityFrameSprite(getNeutralTexture()),
      placementType(placement)
{
    this->name = name;
    this->hp = hp;
    this->dmg = dmg;
    this->cost = cost;
    this->rarity = rarity;
    this->baseAttack = base;
    this->modifiers = mods;
    this->description = description;
    this->faceUp = true;
    this->hitCount = hits;

    if (baseAttack == BaseAttack::Jelepeno) {
        ignoresTileRules = true;
    }

    this->texture = &tex;
    this->sprite = sf::Sprite(tex);

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

sf::IntRect Card::getArtRect() const {
    return sf::IntRect({ 145,   // x
    345},    // y
    {1110,   // width
    1110});// height
}

void Card::applyVisualMode() {
    if (!texture) return;

    sprite.setTexture(*texture, true);
    // uzimamo samo artwork
    sprite.setTextureRect(getArtRect());

    // reset transformacija
    sprite.setScale({1.f, 1.f});
    //sprite.setPosition({100.f,100.f});
    //sprite.setOrigin({0.f, 0.f});
}


void Card::setBrightness(bool highlighted)
{
    if (highlighted)
        sprite.setColor(sf::Color(255, 255, 200)); // SVETLIJE
    else
        sprite.setColor(sf::Color::White);          // NORMALNO
}
void Card::resetVisuals()
{
    sprite.setColor(sf::Color::White);
}