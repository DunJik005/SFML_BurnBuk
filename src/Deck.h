//
// Created by Dun Jik on 12/14/2025.
//

#ifndef SFMLPROJECT_DECK_H
#define SFMLPROJECT_DECK_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Card.h"
#include "CardHand.h"
#include "Hand.h"
#include "CardDataBase.h"

class Deck {

public:

    Deck(CardDataBase& db);

    void setPosition(float x, float y);
    void draw(sf::RenderWindow &window);
    bool contains(float x, float y);


    bool drawCardToHand(Hand& hand, float windowWidth, float windowHeight, Owner owner);

    Owner getOwner() const {return owner;}

private:
    sf::Texture deckTexture;
    sf::Sprite deckSprite;

    CardDataBase& database;

    std::vector<std::shared_ptr<Card>> cards;

    Owner owner;
};

#endif //SFMLPROJECT_DECK_H