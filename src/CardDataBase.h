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
    sf::Texture chapTex;
    sf::Texture ringloTex;
    sf::Texture konstTex;
    sf::Texture nikoTex;
    sf::Texture prikolicaTex;
    sf::Texture slaninaTex;
    sf::Texture ninaTex;
    sf::Texture rotvajlerTex;
    sf::Texture splinterTex;
    sf::Texture ruzicaTex;
    sf::Texture jasnaTex;
    std::vector<std::shared_ptr<Card>> allCards;
    std::vector<std::shared_ptr<sf::Texture>> allTextures; // čuvamo životni vek
    std::mt19937 rng{std::random_device{}()}; // jedan rng za celu bazu

    void initCards() {


        if (!dorotTex.loadFromFile("assets/cards/dorot.jpg"))
        {
            std::cerr << "Greska pri ucitavanju dorota!\n";
        }
        allCards.push_back(std::make_shared<Card>("Dorot",dorotTex, 10, 10, 0, Rarity::Common, BaseAttack::Catapult, AttackModifier::Rapid, 5));
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
        allCards.push_back(std::make_shared<Card>("Ivana Dzarka",dzarkaTex, 0, 3, 1, Rarity::Rare, BaseAttack::Jelepeno, AttackModifier::None, 1, ""));
        if (!tijanaTex.loadFromFile("assets/cards/gladijator.jpg"))
        {
            std::cerr << "Greska pri ucitavanju tijane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Tijana Gladijator", tijanaTex,15,15,0, Rarity::Common, BaseAttack::Catapult, AttackModifier::Reverse));
        if (!grbaviTex.loadFromFile("assets/cards/grbavi.jpg"))
        {
            std::cerr << "Greska pri ucitavanju mine!\n";
        }
        allCards.push_back(std::make_shared<Card>("Mina Grbavi", grbaviTex,4,5,0, Rarity::Common, BaseAttack::Peashooter, AttackModifier::Tall));
        if (!kepecTex.loadFromFile("assets/cards/miloskepec.jpg"))
        {
            std::cerr << "Greska pri ucitavanju tijane!\n";
        }
        allCards.push_back(std::make_shared<Card>("Milos Kepec", kepecTex,7,0,0, Rarity::Common, BaseAttack::Pijavica));
        if (!nosferatuTex.loadFromFile("assets/cards/nosferatu.jpg"))
        {
            std::cerr << "Greska pri ucitavanju nosferatua!\n";
        }
        allCards.push_back(std::make_shared<Card>("Djole Nosferatu", nosferatuTex, 13, 10, 0, Rarity::Epic, BaseAttack::Peashooter, AttackModifier::Wide));
        if (!rtdsefTex.loadFromFile("assets/cards/rtdsef.jpg"))
        {
            std::cerr << "Greska pri ucitavanju rtd sefa!\n";
        }
        allCards.push_back(std::make_shared<Card>("Sef Katedre", rtdsefTex, 14, 7, 0, Rarity::Rare, BaseAttack::Catapult, AttackModifier::Zlatni,1,"Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut Lazar.Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut vestibulum turpis dui eu mi. Vestibulum consequat turpis elit, nec sodales risus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla eleifend, massa ut laoreet sagittis,nisi dui pretium nibh, ut "));

        if (!martamajmunTex.loadFromFile("assets/cards/martamajmun.jpg"))
        {
            std::cerr << "Greska pri ucitavanju marte majmunice!\n";
        }
        allCards.push_back(std::make_shared<Card>("Marta Majmunica", martamajmunTex, 1, 1, 0, Rarity::Rare, BaseAttack::Peashooter, AttackModifier::Zlatni));
        if (!chapTex.loadFromFile("assets/cards/chapika.jpg"))
        {
            std::cerr << "Greska pri ucitavanju chapike!\n";
        }
        allCards.push_back(std::make_shared<Card>("Chapika", chapTex, 13, 0, 0, Rarity::Epic, BaseAttack::Pump));
        if (!ringloTex.loadFromFile("assets/cards/ringlo.jpg"))
        {
            std::cerr << "Greska pri ucitavanju riglo!\n";
        }
        allCards.push_back(std::make_shared<Card>("Kristina Ringlo", ringloTex, 13, 0, 0, Rarity::Rare, BaseAttack::Pump));
        if (!konstTex.loadFromFile("assets/cards/konstantin.jpg"))
        {
            std::cerr << "Greska pri ucitavanju Konstantina!\n";
        }
        allCards.push_back(std::make_shared<Card>("Konstantin Dajamond", konstTex, 3, 0, 0, Rarity::Common, BaseAttack::Pump));
        if (!nikoTex.loadFromFile("assets/cards/nikocado.jpg"))
        {
            std::cerr << "Greska pri ucitavanju Nokocada!\n";
        }
        allCards.push_back(std::make_shared<Card>("Nikocado Avocado", nikoTex, 50, 0, 0, Rarity::Epic, BaseAttack::Pump));
        if (!prikolicaTex.loadFromFile("assets/cards/prikolica.jpg"))
        {
            std::cerr << "Greska pri ucitavanju prikolice!\n";
        }
        allCards.push_back(std::make_shared<Card>("Nikolica Prikolica", prikolicaTex, 12, 7, 0, Rarity::Epic, BaseAttack::Sniper, AttackModifier::Rapid, 5));
        if (!slaninaTex.loadFromFile("assets/cards/slanina.jpg"))
        {
            std::cerr << "Greska pri ucitavanju slanine!\n";
        }
        allCards.push_back(std::make_shared<Card>("Slanina", slaninaTex, 11, 0, 0, Rarity::Rare, BaseAttack::Pump));
        if (!ninaTex.loadFromFile("assets/cards/nina.jpg"))
        {
            std::cerr << "Greska pri ucitavanju nine barbie!\n";
        }
        allCards.push_back(std::make_shared<Card>("Phesic Nina", ninaTex, 7, 13, 0, Rarity::Epic, BaseAttack::Sniper, AttackModifier::Rapid, 5));
        if (!rotvajlerTex.loadFromFile("assets/cards/rotvajler.jpg"))
        {
            std::cerr << "Greska pri ucitavanju rotvajlera!\n";
        }
        allCards.push_back(std::make_shared<Card>("Rotvajler", rotvajlerTex, 14, 10, 0, Rarity::Epic, BaseAttack::Chomper, AttackModifier::Zlatni));
        if (!splinterTex.loadFromFile("assets/cards/splinter.jpg"))
        {
            std::cerr << "Greska pri ucitavanju Splintera!\n";
        }
        allCards.push_back(std::make_shared<Card>("Sasa Splinter", splinterTex, 8, 1, 0, Rarity::Rare, BaseAttack::Catapult, AttackModifier::Rapid, 12));
        if (!ruzicaTex.loadFromFile("assets/cards/ruzica.jpg"))
        {
            std::cerr << "Greska pri ucitavanju Ruzice!\n";
        }
        allCards.push_back(std::make_shared<Card>("Ruzica Mozzart", ruzicaTex, 4, 2, 0, Rarity::Common, BaseAttack::Peashooter, AttackModifier::Laser));
        if (!jasnaTex.loadFromFile("assets/cards/jasna.jpg"))
        {
            std::cerr << "Greska pri ucitavanju Jasne!\n";
        }
        allCards.push_back(std::make_shared<Card>("Jasna's Basics", jasnaTex, 3, 3, 0, Rarity::Common, BaseAttack::Peashooter, AttackModifier::Reverse));
        /*for (auto& card : allCards)
        {
            card->rebuildDescription();
        }*/
    }
};


#endif //SFMLPROJECT_CARDDATABASE_H