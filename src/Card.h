#ifndef SFMLPROJECT_CARD_H
#define SFMLPROJECT_CARD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>

enum class Owner {
    Player1,
    Player2
};

enum class Rarity { Common, Rare, Epic, Legendary, Hero };

enum class AttackType { Chomper, Peashooter, Catapult, Special, Jelepeno, Nut, Tall_Nut,
    Pijavica, Support, Event, Sniper, Imitater, WideChomper, Zlatni_Chomper, LaserPeashooter, Zlatni_Peashooter, RapidPeashooter, ReversePeashooter, ReverseCatapult,
    Laser_Catapult, Catapult_Support, PlotTwist, Arena_Sniper, PlotTwist_Chomper, PlotTwist_Catapult, PlotTwist_Special, PlotTwist_Nut, PlotTwist_Jelepeno,
    PlotTwist_RapidPeashooter, Peashooter_Support, Rapid_Laser_Peashooter, Ker_ili_Zivotinja, Laser_Peashooter_ILI_Zlatni_Chomper, Srpski_Gad, PlotTwist_RapidPeashooter_Support,
    Nut_Support, Event_Nut, Pijavica_Special, Zlatni_Sniper, Hemisfera, Protagonista, };


class Ability {
public:
    virtual void activate() = 0; // virtualna funkcija koju svaka ability override-uje
    virtual ~Ability() = default;
};



class Card {
private:
    std::string name;
    int hp;
    int dmg;
    int cost;
    Rarity rarity;
    bool faceUp;
    AttackType attackType;
    std::string description;
    int cooldown = 0;
    int stunned = 0;
    Owner owner;



    sf::Texture* texture = nullptr; // pokazivač na texture
    //std::shared_ptr<sf::Texture> texture;

    sf::Sprite sprite;

    //sf::Texture rarityFrameTexture;
    sf::Sprite rarityFrameSprite;

    std::vector<std::shared_ptr<Ability>> abilities;






public:
    Card(std::string name, sf::Texture& tex, int hp = 1, int dmg = 1, int cost = 1,
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
    void setOwner(Owner o){ owner = o;}
    void setName(std::string n){ name = n;}
    void setCooldown(int cd){ cooldown = cd;}
    void setStunDuration(int stun){ stunned  = stun;}

    // dodata metoda za setSpritePosition, mozda bude mogo i obican setPosition
    void setSpritePosition(float x, float y) { sprite.setPosition({x, y}); }

    void setBrightness(bool value);
    void resetVisuals();


    void flip() { faceUp = !faceUp; }

    void setTexture(sf::Texture& tex) {
        this->texture = &tex;
        //texture = std::make_shared<sf::Texture>(tex);       // čuvamo adresu texture
        sprite.setTexture(tex);
    }






    void addAbility(std::shared_ptr<Ability> ability) { abilities.push_back(ability); }




    int getHP() const { return hp; }
    int getDamage() const { return dmg; }
    int getCost() const { return cost; }
    Rarity getRarity() const { return rarity; }
    AttackType getAttackType() const { return attackType; }
    const std::string& getDescription() const { return description; }
    const sf::Sprite& getSprite() const { return sprite; }
    sf::Sprite& getSprite(){ return sprite; }
    const sf::Sprite& getFrameSprite() const {return rarityFrameSprite;}
    std::vector<std::shared_ptr<Ability>>& getAbilities() { return abilities; }
    const sf::Texture* getTexture() const { return texture; }
    Owner getOwner() const{return owner;}
    std::string getName()const{return name;}
    int getCooldown() const{ return cooldown;}
    int getStunDuration() const{return stunned;}


    static sf::Texture& getRarityFrame(Rarity r);




    // u card.h, unutar class Card { public: ... }
    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    sf::FloatRect getBounds() const { return sprite.getGlobalBounds(); }

    // omogućimo setovanje pozicije i frame-a iz izvedenih klasa
    //void setSpritePosition(float x, float y) { sprite.setPosition(x, y); }
    sf::Vector2f getPosition() const { return sprite.getPosition(); }
};

#endif // SFMLPROJECT_CARD_H