//
// Created by lazab on 12/22/2025.
//

#include "GameController.h"


bool GameController::handleDeckClick(Hand& hand, float windowWidth, float windowHeight) {
    switch (currentPhase) {
        case GamePhase::Player1Place:
            std::cout << "[GC] Player1 vuče kartu\n";
            return deck.drawCardToHand(hand, windowWidth, windowHeight, getCurrentPlayer());

        case GamePhase::Player2Place:
            std::cout << "[GC] Player2 vuče kartu\n";
            return deck.drawCardToHand(hand, windowWidth, windowHeight, getCurrentPlayer());

        default:
            std::cout << "[GC] Ne može se vući karta u ovoj fazi\n";
            return false;
    }
}

void GameController::sendHandCardToGraveyard(Hand &hand, Graveyard &graveyard, std::shared_ptr<CardHand> &card, float windowWidth) {
    if (!card) return;
    graveyard.addCard(card);
    hand.removeHand(card, windowWidth);

    if (getCurrentPlayer() == Owner::Player1) {
        player1.addElixir();
        std::cout << "Player1 dobija elixir, trenutno: " << player1.getElixir() << "\n";
    } else if (getCurrentPlayer() == Owner::Player2) {
        player2.addElixir();
        std::cout << "Player2 dobija elixir, trenutno: " << player2.getElixir() << "\n";
    }

    card.reset();
}

void GameController::sendBoardCardToGraveyard(Board &board, Graveyard &graveyard, std::shared_ptr<CardBoard> &card) {
    if (!card) return;
    int tile = card->tileIndex;
    auto removed = board.removeCardAt(tile);

    if (removed)
        graveyard.addCard(removed);

    card.reset();

}



Owner GameController::getCurrentPlayer() const {
    switch (currentPhase) {
        case GamePhase::Player1Place: return Owner::Player1;
        case GamePhase::Player2Place: return Owner::Player2;
        default: return Owner::Player1; // ili None
    }
}