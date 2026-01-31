#ifndef BURNBOOKALFA_INTERACTIONCONTROLLER_H
#define BURNBOOKALFA_INTERACTIONCONTROLLER_H

#include <memory>
#include "Board.h"
#include "Hand.h"
#include "Graveyard.h"
#include "GameController.h"
#include "CardView.h"
#include "Player.h"
#include "Deck.h"
#include "Tile.h"

class InteractionController {
public:
    void handleLeftClick(
        int mouseX, int mouseY,
        Board& board,
        Deck& deck,
        Hand& activeHand,
        GameController& gameController,
        Graveyard& graveyard,
        Player& player1,
        Player& player2,
        float windowWidth,
        float windowHeight
    );

    void handleRightClick(CardView& cardView);

    void clearSelection();

    // GETTERS
    std::shared_ptr<CardHand> getSelectedHandCard() const;
    std::shared_ptr<Card> getSelectedBoardCard() const;


private:
    std::shared_ptr<CardHand> selectedHandCard = nullptr;
    std::shared_ptr<Card>     selectedBoardCard = nullptr;
    int selectedRow = -1;
    int selectedCol = -1;
};


#endif //BURNBOOKALFA_INTERACTIONCONTROLLER_H