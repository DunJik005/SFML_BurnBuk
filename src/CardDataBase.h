//
// Created by lazab on 12/22/2025.
//

#ifndef SFMLPROJECT_CARDDATABASE_H
#define SFMLPROJECT_CARDDATABASE_H

#include "Card.h"
#include <vector>
#include <memory>
#include <random>
#include <SFML/Graphics.hpp>
#include <iostream>


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
    std::vector<std::shared_ptr<Card>> allCards;
    std::vector<std::shared_ptr<sf::Texture>> allTextures; // čuvamo životni vek
    std::mt19937 rng{std::random_device{}()}; // jedan rng za celu bazu

    void initCards() {


        if (!dorotTex.loadFromFile("assets/cards/dorot.jpg"))
        {
            std::cerr << "Greska pri ucitavanju dorota!\n";
        }
        allCards.push_back(std::make_shared<Card>("Dorot",dorotTex, 0, 10, 0, Rarity::Common, AttackType::Chomper));
        if (!homoTex.loadFromFile("assets/cards/homohabilis.jpg"))
        {
            std::cerr << "Greska pri ucitavanju homohabilisa!\n";
        }
        allCards.push_back(std::make_shared<Card>("Homohabilis",homoTex, 6, 6, 0, Rarity::Common, AttackType::Chomper));
        if (!hrkTex.loadFromFile("assets/cards/hrk man.jpg"))
        {
            std::cerr << "Greska pri ucitavanju hrkmana!\n";
        }
        allCards.push_back(std::make_shared<Card>("Hrkman Piksla",hrkTex, 5, 3, 0, Rarity::Common, AttackType::Peashooter));
        if (!jajaTex.loadFromFile("assets/cards/plavajaja.jpg"))
        {
            std::cerr << "Greska pri ucitavanju plavihjaja!\n";
        }
        allCards.push_back(std::make_shared<Card>("Kapsula",jajaTex, 12, 0, 0, Rarity::Common, AttackType::Nut));
        if (!dzarkaTex.loadFromFile("assets/cards/ivana.jpg"))
        {
            std::cerr << "Greska pri ucitavanju ivane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Ivana Dzarka",dzarkaTex, 0, 3, 1, Rarity::Rare, AttackType::Jelepeno));
        if (!tijanaTex.loadFromFile("assets/cards/gladijator.jpg"))
        {
            std::cerr << "Greska pri ucitavanju tijane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Tijana Gladijator", tijanaTex,5,5,0, Rarity::Common, AttackType::Chomper));
        if (!grbaviTex.loadFromFile("assets/cards/grbavi.jpg"))
        {
            std::cerr << "Greska pri ucitavanju mine!\n";
        }
        allCards.push_back(std::make_shared<Card>("Mina Grbavi", grbaviTex,4,5,0, Rarity::Common, AttackType::Peashooter));
        if (!kepecTex.loadFromFile("assets/cards/miloskepec.jpg"))
        {
            std::cerr << "Greska pri ucitavanju tijane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Milos Kepec", kepecTex,7,0,0, Rarity::Common, AttackType::Pijavica));
    }
};


#endif //SFMLPROJECT_CARDDATABASE_H