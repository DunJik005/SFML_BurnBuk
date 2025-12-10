#ifndef SFMLPROJECT_CARD_H
#define SFMLPROJECT_CARD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>

enum class Rarity { Common, Rare, Epic, Legendary, Hero };

enum class AttackType { Chomper, Peashooter, Catapult /* ... ostali tipovi */ };


class Ability {
public:
    virtual void activate() = 0; // virtualna funkcija koju svaka ability override-uje
    virtual ~Ability() = default;
};

class Card {
private:
    int hp;
    int dmg;
    int cost;
    Rarity rarity;
    bool faceUp;
    AttackType attackType;
    std::string description;



    sf::Texture* texture = nullptr; // pokazivač na texture
    sf::Sprite sprite;

    sf::Texture rarityFrameTexture;
    sf::Sprite rarityFrameSprite;

    std::vector<std::shared_ptr<Ability>> abilities;


public:

    // Konstruktor sa podrazumevanim vrednostima
    Card(sf::Texture& tex, int hp = 1, int dmg = 1, int cost = 1,
         Rarity rarity = Rarity::Common,
         AttackType attackType = AttackType::Chomper,
         const std::string& description = "");

    // Setters
    void setHP(int h) { hp = h; }
    void setDamage(int d) { dmg = d; }
    void setCost(int c) { cost = c; }
    void setRarity(Rarity r) { rarity = r; }
    void setAttackType(AttackType a) { attackType = a; }
    void setDescription(const std::string& desc) { description = desc; }
    void setPosition(float x, float y) { sprite.setPosition({x, y}); }
    void flip() { faceUp = !faceUp; }

    void setTexture(sf::Texture& tex) {
        texture = &tex;       // čuvamo adresu texture
        sprite.setTexture(tex);
    }

    void addAbility(std::shared_ptr<Ability> ability) { abilities.push_back(ability); }

    // Getters
    int getHP() const { return hp; }
    int getDamage() const { return dmg; }
    int getCost() const { return cost; }
    Rarity getRarity() const { return rarity; }
    AttackType getAttackType() const { return attackType; }
    const std::string& getDescription() const { return description; }
    const sf::Sprite& getSprite() const { return sprite; }
    std::vector<std::shared_ptr<Ability>>& getAbilities() { return abilities; }
};

#endif // SFMLPROJECT_CARD_H