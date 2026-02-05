//
// Created by lazab on 12/22/2025.
//

#ifndef SFMLPROJECT_CARDDATABASE_H
#define SFMLPROJECT_CARDDATABASE_H

#include "Card.h"
#include <vector>
#include <memory>
#include <random>
#include <map>
#include <string>
#include <SFML/Graphics.hpp>
#include <iostream>

extern "C" {
    #include "SQLite/sqlite3.h"
}


inline AttackModifier operator|(AttackModifier a, AttackModifier b)
{
    return static_cast<AttackModifier>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b)
    );
}

inline AttackModifier& operator|=(AttackModifier& a, AttackModifier b)
{
    a = a | b;
    return a;
}




class CardDataBase {
public:
    CardDataBase() {
        initCards();
    }

    const std::vector<std::shared_ptr<Card>>& getAllCards()const {
        return allCards;
    }

private:
    sf::Texture dorotTex;
    sf::Texture homoTex;
    sf::Texture hrkTex;
    sf::Texture jajaTex;
    sf::Texture dzarkaTex;
    sf::Texture tijanaTex;
    sf::Texture grbaviTex;
    sf::Texture kepecTex;
    sf::Texture nosferatuTex;
    sf::Texture rtdsefTex;
    sf::Texture martamajmunTex;
    std::vector<std::shared_ptr<Card>> allCards;
    std::vector<std::shared_ptr<sf::Texture>> allTextures; // čuvamo životni vek
    std::mt19937 rng{std::random_device{}()}; // jedan rng za celu bazu

    void initCards() {
        sqlite3* baza;
        int return_code = sqlite3_open("burnbook.db", &baza);
        if (return_code) std::cout << "Baza ne moze da se otvori!" << std::endl;
        else std::cout << "Povezan na bazu!" << std::endl;

        if (!dorotTex.loadFromFile("assets/cards/dorot.jpg"))
        {
            std::cerr << "Greska pri ucitavanju dorota!\n";
        }
        allCards.push_back(std::make_shared<Card>("Dorot",dorotTex, 0, 10, 0, Rarity::Common, BaseAttack::Chomper));
        if (!homoTex.loadFromFile("assets/cards/homohabilis.jpg"))
        {
            std::cerr << "Greska pri ucitavanju homohabilisa!\n";
        }
        allCards.push_back(std::make_shared<Card>("Homohabilis",homoTex, 6, 6, 0, Rarity::Common, BaseAttack::Chomper));
        if (!hrkTex.loadFromFile("assets/cards/hrk man.jpg"))
        {
            std::cerr << "Greska pri ucitavanju hrkmana!\n";
        }
        allCards.push_back(std::make_shared<Card>("Hrkman Piksla",hrkTex, 5, 3, 0, Rarity::Common, BaseAttack::Peashooter, AttackModifier::None, 1, "[PEASHOOTER]\nAbility 1: Pljugica\nLights one up and blows smoke into enemy's face" ));
        if (!jajaTex.loadFromFile("assets/cards/plavajaja.jpg"))
        {
            std::cerr << "Greska pri ucitavanju plavihjaja!\n";
        }
        allCards.push_back(std::make_shared<Card>("Kapsula",jajaTex, 12, 0, 0, Rarity::Common, BaseAttack::Nut));
        if (!dzarkaTex.loadFromFile("assets/cards/ivana.jpg"))
        {
            std::cerr << "Greska pri ucitavanju ivane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Ivana Dzarka",dzarkaTex, 0, 3, 1, Rarity::Rare, BaseAttack::Jelepeno));
        if (!tijanaTex.loadFromFile("assets/cards/gladijator.jpg"))
        {
            std::cerr << "Greska pri ucitavanju tijane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Tijana Gladijator", tijanaTex,5,5,0, Rarity::Common, BaseAttack::Chomper));
        if (!grbaviTex.loadFromFile("assets/cards/grbavi.jpg"))
        {
            std::cerr << "Greska pri ucitavanju mine!\n";
        }
        allCards.push_back(std::make_shared<Card>("Mina Grbavi", grbaviTex,4,5,9, Rarity::Common, BaseAttack::Peashooter));
        if (!kepecTex.loadFromFile("assets/cards/miloskepec.jpg"))
        {
            std::cerr << "Greska pri ucitavanju tijane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Milos Kepec", kepecTex,7,0,0, Rarity::Common, BaseAttack::Pijavica));
        if (!nosferatuTex.loadFromFile("assets/cards/nosferatu.jpg"))
        {
            std::cerr << "Greska pri ucitavanju nosferatua!\n";
        }
        allCards.push_back(std::make_shared<Card>("Djole Nosferatu", nosferatuTex, 13, 4, 5, Rarity::Epic, BaseAttack::Peashooter, AttackModifier::Reverse));
        if (!rtdsefTex.loadFromFile("assets/cards/rtdsef.jpg"))
        {
            std::cerr << "Greska pri ucitavanju rtd sefa!\n";
        }
        allCards.push_back(std::make_shared<Card>("Sef Katedre", rtdsefTex, 14, 7, 6, Rarity::Hero, BaseAttack::Catapult, AttackModifier::Zlatni,1,"Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. "));

        if (!martamajmunTex.loadFromFile("assets/cards/martamajmun.jpg"))
        {
            std::cerr << "Greska pri ucitavanju marte majmunice!\n";
        }
        allCards.push_back(std::make_shared<Card>("Marta Majmunica", martamajmunTex, 1, 1, 0, Rarity::Rare, BaseAttack::Peashooter, AttackModifier::Zlatni));
    }
};


#endif //SFMLPROJECT_CARDDATABASE_H