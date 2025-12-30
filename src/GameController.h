#ifndef SFMLPROJECT_GAMECONTROLLER_H
#define SFMLPROJECT_GAMECONTROLLER_H

#include "Board.h"
#include "HandleAttack.h"
#include "TurnButtons.h"
#include "Graveyard.h"

enum class GamePhase {
    Player1Place,
    Player2Place,
    ResolveAttacks
};

class GameController {
public:
    GameController(Board& b, TurnButtons& buttons, Deck& deck, Player& p1, Player& p2):
    board(b),
    attackHandler(board),
    currentPhase(GamePhase::Player1Place),
    deck(deck),
    player1TurnDone(false),
    player2TurnDone(false),
    player1(p1),
    player2(p2),
    turnButtons(buttons){}


    void drawCardForCurrentPlayer(Hand &hand, float windowWidth, float windowHeight) {
        switch (currentPhase) {
            case GamePhase::Player1Place:
                deck.drawCardToHand(hand, windowWidth, windowHeight, getCurrentPlayer());
                break;
            case GamePhase::Player2Place:
                deck.drawCardToHand(hand, windowWidth, windowHeight, getCurrentPlayer());
                break;
            default:
                break;
        }
    }

    void sendHandCardToGraveyard(Hand& hand,
        Graveyard& graveyard,
        std::shared_ptr<CardHand>& card,
        float windowWidth);

    void sendBoardCardToGraveyard(
        Board& board,
        Graveyard& graveyard,
        std::shared_ptr<CardBoard>& card
    );


    void update() {
        switch (currentPhase) {

            case GamePhase::Player1Place:
                if (player1TurnDone) {
                    std::cout << "Player1 je zavrsio potez\n";
                    currentPhase = GamePhase::Player2Place;
                    player1TurnDone = false; // reset flag
                }
                break;

            case GamePhase::Player2Place:
                if (player2TurnDone) {
                    std::cout << "Player2 je zavrsio potez\n";
                    currentPhase = GamePhase::ResolveAttacks;
                    player2TurnDone = false; // reset flag
                }
                break;

            case GamePhase::ResolveAttacks:
                std::cout << "Napadi pocinju\n";

                attackHandler.executeAttacks();

                // nakon napada, resetujemo flagove i prelazimo na Player1Place
                currentPhase = GamePhase::Player1Place;
                player1TurnDone = false;
                player2TurnDone = false;
                std::cout << "Pocinje potez Player1\n";
                break;
        }
    }

    GamePhase getCurrentPhase() const { return currentPhase; }

    void setPlayer1Done(bool done) { player1TurnDone = done; }

    void setPlayer2Done(bool done) { player2TurnDone = done; }

    bool handleDeckClick(Hand& hand, float windowWidth, float windowHeight);

    Owner getCurrentPlayer()const;

private:
    Player& player1;
    Player& player2;

    Board& board;
    HandleAttack attackHandler;
    TurnButtons& turnButtons;
    GamePhase currentPhase;
    bool player1TurnDone;
    bool player2TurnDone;
    Deck& deck;


    // stub funkcije – ovde ćeš dodati pravu logiku za input igrača
    bool player1DonePlacing() const { return player1TurnDone; }
    bool player2DonePlacing() const { return player2TurnDone; }



};

#endif //SFMLPROJECT_GAMECONTROLLER_H