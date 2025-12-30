//
// Created by Dun Jik on 12/12/2025.
//

#ifndef SFMLPROJECT_CARDBOARD_H
#define SFMLPROJECT_CARDBOARD_H
#include "Card.h"




class CardBoard : public Card {
public:
    using Card::Card;

    int tileIndex = -1;




    void draw(sf::RenderWindow &window) override {
        Card::draw(window);
        // kasnije da se dodaju i brojevi za dmg i hp u ivicama karte
    }
};


#endif //SFMLPROJECT_CARDBOARD_H