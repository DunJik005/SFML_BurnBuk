#ifndef SFMLPROJECT_CARD_H
#define SFMLPROJECT_CARD_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <vector>

enum class Owner {
    Player1,
    Player2,
    None
};

enum class PlacementType {
    Normal,        // standardna karta (na svoj tile)
    Parasite,      // kači se na enemy kartu (stack)
    Infiltrator    // ide na prazno enemy polje
};


enum class Rarity { Common, Rare, Epic, Legendary, Hero };

inline Rarity stringToRarity(const std::string& str) {
    if (str == "Common")    return Rarity::Common;
    if (str == "Rare")      return Rarity::Rare;
    if (str == "Epic")      return Rarity::Epic;
    if (str == "Legendary") return Rarity::Legendary;
    return Rarity::Common; // Default vrednost
}

enum class AttackType { Chomper, Peashooter, Catapult, Special, Jelepeno, Nut, Tall_Nut,
    Pijavica, Support, Event, Sniper, Imitater, WideChomper, Zlatni_Chomper, LaserPeashooter, Zlatni_Peashooter, RapidPeashooter, ReversePeashooter, ReverseCatapult,
    Laser_Catapult, Catapult_Support, PlotTwist, Arena_Sniper, PlotTwist_Chomper, PlotTwist_Catapult, PlotTwist_Special, PlotTwist_Nut, PlotTwist_Jelepeno,
    PlotTwist_RapidPeashooter, Peashooter_Support, Rapid_Laser_Peashooter, Ker_ili_Zivotinja, Laser_Peashooter_ILI_Zlatni_Chomper, Srpski_Gad, PlotTwist_RapidPeashooter_Support,
    Nut_Support, Event_Nut, Pijavica_Special, Zlatni_Sniper, Hemisfera, Protagonista, };


inline AttackType stringToAttackType(const std::string& str) {
    static const std::unordered_map<std::string, AttackType> stringToEnumMap = {
        {"Chomper", AttackType::Chomper},
        {"Peashooter", AttackType::Peashooter},
        {"Catapult", AttackType::Catapult},
        {"Special", AttackType::Special},
        {"Jelepeno", AttackType::Jelepeno},
        {"Nut", AttackType::Nut},
        {"Tall Nut", AttackType::Tall_Nut},
        {"Pijavica", AttackType::Pijavica},
        {"Support", AttackType::Support},
        {"Event", AttackType::Event},
        {"Sniper", AttackType::Sniper},
        {"Imitater", AttackType::Imitater},
        {"WideChomper", AttackType::WideChomper},
        {"Zlatni Chomper", AttackType::Zlatni_Chomper},
        {"LaserPeashooter", AttackType::LaserPeashooter},
        {"Zlatni Peashooter", AttackType::Zlatni_Peashooter},
        {"RapidPeashooter", AttackType::RapidPeashooter},
        {"ReversePeashooter", AttackType::ReversePeashooter},
        {"ReverseCatapult", AttackType::ReverseCatapult},
        {"Laser Catapult", AttackType::Laser_Catapult},
        {"Catapult Support", AttackType::Catapult_Support},
        {"PlotTwist", AttackType::PlotTwist},
        {"Arena Sniper", AttackType::Arena_Sniper},
        {"PlotTwist Chomper", AttackType::PlotTwist_Chomper},
        {"PlotTwist Catapult", AttackType::PlotTwist_Catapult},
        {"PlotTwist Special", AttackType::PlotTwist_Special},
        {"PlotTwist Nut", AttackType::PlotTwist_Nut},
        {"PlotTwist Jelepeno", AttackType::PlotTwist_Jelepeno},
        {"PlotTwist RapidPeashooter", AttackType::PlotTwist_RapidPeashooter},
        {"Peashooter Support", AttackType::Peashooter_Support},
        {"Rapid Laser Peashooter", AttackType::Rapid_Laser_Peashooter},
        {"Ker ili Zivotinja", AttackType::Ker_ili_Zivotinja},
        {"Laser Peashooter ILI Zlatni Chomper", AttackType::Laser_Peashooter_ILI_Zlatni_Chomper},
        {"Srpski Gad", AttackType::Srpski_Gad},
        {"PlotTwist RapidPeashooter Support", AttackType::PlotTwist_RapidPeashooter_Support},
        {"Nut Support", AttackType::Nut_Support},
        {"Event Nut", AttackType::Event_Nut},
        {"Pijavica Special", AttackType::Pijavica_Special},
        {"Zlatni Sniper", AttackType::Zlatni_Sniper},
        {"Hemisfera", AttackType::Hemisfera},
        {"Protagonista", AttackType::Protagonista}
    };

    auto it = stringToEnumMap.find(str);
    if (it != stringToEnumMap.end()) {
        return it->second;
    }

    // Default vrednost ako string nije pronađen
    return AttackType::Special;
}

// dodatak za mergovanje razlicitih keywordova na base napade
enum class BaseAttack {
    Chomper, Peashooter, Catapult, Nut, Jelepeno, Pijavica
};

inline BaseAttack stringToBaseAttack(const std::string& str) {
    static const std::unordered_map<std::string, BaseAttack> stringToEnumMap = {
        {"Chomper",    BaseAttack::Chomper},
        {"Peashooter", BaseAttack::Peashooter},
        {"Catapult",   BaseAttack::Catapult},
        {"Nut",        BaseAttack::Nut},
        {"Jelepeno",   BaseAttack::Jelepeno},
        {"Pijavica",   BaseAttack::Pijavica}
    };

    auto it = stringToEnumMap.find(str);
    if (it != stringToEnumMap.end()) {
        return it->second;
    }

    // Default vrednost ako string u bazi ne odgovara nijednom tipu
    return BaseAttack::Chomper;
}

enum class AttackModifier : uint32_t {
    None    = 0,
    Laser   = 1 << 0,
    Rapid   = 1 << 1,
    Zlatni  = 1 << 2,
    Reverse = 1 << 3,
    Wide    = 1 << 4
};

inline AttackModifier stringToModifiers(const std::string& str) {
    uint32_t result = 0;
    std::stringstream ss(str);
    std::string segment;

    while (std::getline(ss, segment, ',')) {
        // Obriši prazna mesta ako postoje
        segment.erase(0, segment.find_first_not_of(" "));
        segment.erase(segment.find_last_not_of(" ") + 1);

        if (segment == "Laser")   result |= static_cast<uint32_t>(AttackModifier::Laser);
        if (segment == "Rapid")   result |= static_cast<uint32_t>(AttackModifier::Rapid);
        if (segment == "Zlatni")  result |= static_cast<uint32_t>(AttackModifier::Zlatni);
        if (segment == "Reverse") result |= static_cast<uint32_t>(AttackModifier::Reverse);
        if (segment == "Wide")    result |= static_cast<uint32_t>(AttackModifier::Wide);
    }

    return static_cast<AttackModifier>(result);
}


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
    int hitCount;
    Owner owner;

    PlacementType placementType = PlacementType::Normal;

    bool ignoresTileRules = false;


    BaseAttack baseAttack;
    AttackModifier modifiers;



    sf::Texture* texture = nullptr; // pokazivač na texture
    //std::shared_ptr<sf::Texture> texture;

    sf::Sprite sprite;

    //sf::Texture rarityFrameTexture;
    sf::Sprite rarityFrameSprite;

    std::vector<std::shared_ptr<Ability>> abilities;






public:
    Card(std::string name, sf::Texture& tex, int hp = 1, int dmg = 1, int cost = 1,
     Rarity rarity = Rarity::Common,
     BaseAttack base = BaseAttack::Chomper,
     AttackModifier mods = AttackModifier::None,
     int hits = 1,
     const std::string& description = "",
     PlacementType placement = PlacementType::Normal);

    // SETTERS
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
    void setBaseAttack(BaseAttack b) { baseAttack = b; }
    void setModifiers(AttackModifier m) { modifiers = m; }
    void setHitCount(int c) { hitCount = c; }
    void setPlacementType(PlacementType p) { placementType = p;}
    void setIgnoresTileRules(bool v) { ignoresTileRules = v; }

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



// GETTERS
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
    BaseAttack getBaseAttack() const { return baseAttack; }
    AttackModifier getModifiers() const { return modifiers; }
    int getHitCount() const{ return hitCount; }
    PlacementType getPlacementType()const{return placementType;}
    bool getIgnoresTileRules() const { return ignoresTileRules; }


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