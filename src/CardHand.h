#ifndef SFMLPROJECT_CARDHAND_H
#define SFMLPROJECT_CARDHAND_H

#include "Card.h"


class CardHand : public Card {
public:
    using Card::Card;

    int handIndex = -1;

    void draw(sf::RenderWindow &window) override {
        Card::draw(window);
    }


    bool containsPoint(float x, float y) {
        return getBounds().contains({x, y});
    }



/*
    static sf::Texture& getCardBackTexture() {
        static sf::Texture back;
        static bool loaded = false;

        if (!loaded) {
            if (!back.loadFromFile("assets/backgroundHand.png")) {
                std::cerr << "Greska pri ucitavanju backgroundHand.png\n";
            }
            loaded = true;
        }
        return back;
    }*/
};


#endif //SFMLPROJECT_CARDHAND_H