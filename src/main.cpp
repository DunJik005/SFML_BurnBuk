#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Board.h"
#include "Card.h"
#include "CardHand.h"
#include "Hand.h"
#include "Deck.h"
#include "Graveyard.h"
#include "Player.h"
#include "CardView.h"
#include "TurnButtons.h"
#include "GameController.h"
#include "InteractionController.h"
#include "CardDataBase.h"
#include "MusicManager.h"
#include "PlayerView.h"

#include <ctime>
#include <cstdlib>

int main() {
    sf::Clock clock;
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Muzika
    MusicManager::instance().loadBackgroundPlaylist("assets/music/background");
    MusicManager::instance().playBackground();

    // Prozori
    sf::RenderWindow window1(sf::VideoMode({1200, 800}), "Player 1");
    sf::RenderWindow window2(sf::VideoMode({1200, 800}), "Player 2");
    window1.setFramerateLimit(60);
    window2.setFramerateLimit(60);

    // Baza i deck
    CardDataBase cardDB;
    Deck gameDeck(cardDB);

    // Igrači
    Player player1;
    Player player2;

    // Board i dugmad
    Board board(1200, 800, player1, player2);
    TurnButtons turnButtons(1200, 800);

    // Game controller
    GameController gameController(board, turnButtons, gameDeck, player1, player2);
    gameDeck.setPosition(board, -80.f);

    // Ruke
    Hand p1Hand, p2Hand;
    p1Hand.onResize(1200, 800);
    p2Hand.onResize(1200, 800);

    // Graveyard
    Graveyard graveyard;
    graveyard.setPosition(board, 80);

    // CardView
    CardView cardView;
    cardView.onResize(1200, 800);

    // Interaction
    InteractionController interaction;

    // === PLAYER VIEW-ovi ===
    PlayerView view1(
        window1, Owner::Player1,
        board, p1Hand, p2Hand,
        gameDeck, graveyard,
        turnButtons, gameController,
        interaction, cardView,
        player1, player2
    );

    PlayerView view2(
        window2, Owner::Player2,
        board, p2Hand, p1Hand,
        gameDeck, graveyard,
        turnButtons, gameController,
        interaction, cardView,
        player1, player2
    );

    // === LOOP ===
    while (window1.isOpen() && window2.isOpen()) {
        float dt = clock.restart().asSeconds();

        gameController.updateHandsState(p1Hand, p2Hand);
        MusicManager::instance().update();

        // EVENTI
        while (auto e = window1.pollEvent())
            view1.handleEvent(*e);

        while (auto e = window2.pollEvent())
            view2.handleEvent(*e);

        // UPDATE
        view1.update(dt);
        view2.update(dt);

        // DRAW
        window1.clear(sf::Color::White);
        view1.draw();
        window1.display();

        window2.clear(sf::Color::White);
        view2.draw();
        window2.display();
    }
    return 0;
}
