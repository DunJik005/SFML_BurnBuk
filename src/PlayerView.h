//
// Created by Dun Jik on 2/13/2026.
//

#ifndef SFMLPROJECT_PLAYERVIEW_H
#define SFMLPROJECT_PLAYERVIEW_H


#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Hand.h"
#include "Deck.h"
#include "Graveyard.h"
#include "TurnButtons.h"
#include "GameController.h"
#include "InteractionController.h"

class PlayerView {
public:
    PlayerView(
        sf::RenderWindow& w,
        Owner myOwner,
        Board& b,
        Hand& my,
        Hand& enemy,
        Deck& d,
        Graveyard& g,
        TurnButtons& btn,
        GameController& gc,
        InteractionController& ic,
        CardView& cv,
        Player& p1,
        Player& p2
    );


    void handleEvent(const sf::Event& e);
    void update(float dt);
    void draw();

private:
    sf::Vector2f mapMouse(int x, int y);

    sf::RenderWindow& window;
    sf::View view;
    Owner owner;

    Board& board;
    Hand& myHand;
    Hand& enemyHand;
    Deck& deck;
    Graveyard& graveyard;
    TurnButtons& buttons;
    GameController& controller;
    InteractionController& interaction;
    CardView& cardView;

    Player& player1;
    Player& player2;

};



#endif //SFMLPROJECT_PLAYERVIEW_H