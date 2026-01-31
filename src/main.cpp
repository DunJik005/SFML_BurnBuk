#include <SFML/Graphics.hpp>
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
#include <iostream>
#include <SFML/Audio.hpp>
using namespace sf;

int main() {
    sf::Clock clock; //meri vreme za skrol

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Music music;
    if (!music.openFromFile("assets/music/POTC.ogg")) {
        std::cout << "Muzika nije ucitana."<< std::endl;
    }
    music.play();
    music.setLooping(true);
    RenderWindow window(
        //VideoMode({1820,1080}),
        VideoMode({1000, 600}),
        "Tedzan uci",
        sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize
    );
// da graficka ne radi na 3000fpsa
    window.setFramerateLimit(60);

    CardDataBase cardDB;

    Deck gameDeck(cardDB);

    Deck* activeDeck = nullptr;


    Player player1;
    Player player2;


    // pravi se board
    Board board(window.getSize().x, window.getSize().y);

    TurnButtons turnButtons(window.getSize().x, window.getSize().y);

    GameController gameController(board, turnButtons, gameDeck, player1, player2);

    gameDeck.setPosition(board, -80.f);


    // pravi se hand objekat
    Hand p1Hand;
    Hand p2Hand;
    p1Hand.onResize(window.getSize().x, window.getSize().y);
    p2Hand.onResize(window.getSize().x, window.getSize().y);

    std::shared_ptr<CardHand> selectedCard = nullptr;

    //pravi se graveyard instanca i selected board karta za graveyard (na foru selectedCarda za hand)
    Graveyard graveyard;
    graveyard.setPosition(board, 80);
    std::shared_ptr<Card> selectedBoardCard = nullptr;


// pravi se card view instanca
    CardView cardView;
    cardView.onResize(window.getSize().x, window.getSize().y);


    InteractionController interaction;

    while (window.isOpen())
    {
        // ===============================
        // 1️⃣ UPDATE HAND STATE (GC)
        // ===============================
        gameController.updateHandsState(p1Hand, p2Hand);

        Hand* activeHand =
            (gameController.getCurrentPlayer() == Owner::Player1)
            ? &p1Hand : &p2Hand;

        Hand* inactiveHand =
            (activeHand == &p1Hand) ? &p2Hand : &p1Hand;

        // ===============================
        // 2️⃣ EVENT LOOP
        // ===============================
        while (auto event = window.pollEvent())
        {
            // --- CLOSE ---
            if (event->is<sf::Event::Closed>())
                window.close();

            // --- SCROLL (CardView) ---
            if (event->is<sf::Event::MouseWheelScrolled>())
            {
                auto* e = event->getIf<sf::Event::MouseWheelScrolled>();
                if (e->wheel == sf::Mouse::Wheel::Vertical)
                    cardView.scrollDescription(e->delta);
            }

            // --- RESIZE ---
            if (event->is<sf::Event::Resized>())
            {
                auto* e = event->getIf<sf::Event::Resized>();

                board.onResize(e->size.x, e->size.y);
                turnButtons.onResize(e->size.x, e->size.y);

                gameDeck.setPosition(board, -80.f); // ✅ DODAJ OVO
                graveyard.setPosition(board, +80.f); // ispod sredine

                p1Hand.onResize(e->size.x, e->size.y);
                p2Hand.onResize(e->size.x, e->size.y);

                cardView.onResize(e->size.x, e->size.y);

                sf::View view(sf::FloatRect(
                    {0.f, 0.f},
                    {static_cast<float>(e->size.x),
                    static_cast<float>(e->size.y)}
                ));
                window.setView(view);
            }

            // --- HOVER ---
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            board.updateHover(mousePos.x, mousePos.y);

            // --- MOUSE BUTTONS ---
            if (event->is<sf::Event::MouseButtonPressed>())
            {
                auto* e = event->getIf<sf::Event::MouseButtonPressed>();
                cardView.hide();

                // LEFT CLICK
                if (e->button == sf::Mouse::Button::Left)
                {
                    // Turn buttons (ostaje isto)
                    Owner current = gameController.getCurrentPlayer();

                    if (current == Owner::Player1 &&
                        turnButtons.handleClick(e->position.x, e->position.y, true))
                    {
                        gameController.setPlayer1Done(true);
                        gameController.update();
                        continue;
                    }

                    if (current == Owner::Player2 &&
                        turnButtons.handleClick(e->position.x, e->position.y, false))
                    {
                        gameController.setPlayer2Done(true);
                        gameController.update();
                        continue;
                    }

                    interaction.handleLeftClick(
                    e->position.x,
                    e->position.y,
                    board,
                    gameDeck,
                    *activeHand,
                    gameController,
                    graveyard,
                    player1,
                    player2,
                    window.getSize().x,
                    window.getSize().y
                );

                }

                // RIGHT CLICK
                if (e->button == sf::Mouse::Button::Right)
                {
                    interaction.handleRightClick(cardView);
                }
            }
        }

        // ===============================
        // 3️⃣ UPDATE
        // ===============================
        float dt = clock.restart().asSeconds();
        cardView.update(dt);

        // ===============================
        // 4️⃣ DRAW
        // ===============================
        window.clear(sf::Color::White);

        board.draw(
            window,
            interaction.getSelectedBoardCard(),
            gameController.getCurrentPlayer()
        );
        inactiveHand->draw(window, nullptr);
        activeHand->draw(window, interaction.getSelectedHandCard());


        gameDeck.draw(window);
        graveyard.draw(window);
        cardView.draw(window);
        turnButtons.draw(window);

        window.display();
    }

    return 0;
}