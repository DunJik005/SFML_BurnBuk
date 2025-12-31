#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Card.h"
#include "CardHand.h"
#include "CardBoard.h"
#include "Hand.h"
#include "Deck.h"
#include "Graveyard.h"
#include "Player.h"
#include "CardView.h"
#include "TurnButtons.h"
#include "GameController.h"
#include "CardDataBase.h"
#include <iostream>

using namespace sf;


int main() {

    RenderWindow window(
        //VideoMode({1820,1080}),
        VideoMode({1000, 800}),
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

    // pravi se neka lokalna custom karta za test
    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile("assets/cards/abortus.jpg"))
    {
        std::cerr << "Greska pri ucitavanju card texture!\n";
    }


    gameDeck.setPosition(board.getDeckPosition().x, board.getDeckPosition().y);


    // pravi se hand objekat
    Hand p1Hand;
    Hand p2Hand;
    p1Hand.onResize(window.getSize().x, window.getSize().y);
    p2Hand.onResize(window.getSize().x, window.getSize().y);

    std::shared_ptr<CardHand> selectedCard = nullptr;

    //pravi se graveyard instanca i selected board karta za graveyard (na foru selectedCarda za hand)
    Graveyard graveyard;
    std::shared_ptr<CardBoard> selectedBoardCard = nullptr;


// pravi se card view instanca
    CardView cardView;
    cardView.onResize(window.getSize().x, window.getSize().y);


    while (window.isOpen())
    {


        Hand* activeHand = nullptr;
        Hand* inactiveHand = nullptr;

        switch (gameController.getCurrentPlayer()) {
            case Owner::Player1:
                activeHand   = &p1Hand;
                inactiveHand = &p2Hand;
                break;

            case Owner::Player2:
                activeHand   = &p2Hand;
                inactiveHand = &p1Hand;
                break;
        }
        activeHand->setActive(true);
        inactiveHand->setActive(false);


        activeHand->setVisibleOwner(gameController.getCurrentPlayer());

        while (auto event = window.pollEvent())
        {
            // Zatvaranje
            if (event->is<Event::Closed>())
                window.close();

            // Resize
            if (event->is<Event::Resized>())
            {
                auto* e = event->getIf<Event::Resized>();
                board.onResize(e->size.x, e->size.y);
                p1Hand.onResize(window.getSize().x, window.getSize().y);
                p2Hand.onResize(window.getSize().x, window.getSize().y);


                //resizovanje cardView karte
                cardView.onResize(e->size.x, e->size.y);

                // Ako želiš da ne rastegne sliku, vec viewport:
                sf::View view(sf::FloatRect(
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(static_cast<float>(e->size.x),
                 static_cast<float>(e->size.y))));

                window.setView(view);
            }

            // Klik
            if (event->is<Event::MouseButtonPressed>())
            {
                auto* e = event->getIf<Event::MouseButtonPressed>();
                if (e->button == sf::Mouse::Button::Left)
                {
                    int mouseX = e->position.x;
                    int mouseY = e->position.y;

                    Owner current = gameController.getCurrentPlayer();

                    if (current == Owner::Player1) {
                        if (turnButtons.handleClick(mouseX, mouseY, true)) {
                            gameController.setPlayer1Done(true);
                            gameController.update();
                            continue;
                        }
                    }
                    else if (current == Owner::Player2) {
                        if (turnButtons.handleClick(mouseX, mouseY, false)) {
                            gameController.setPlayer2Done(true);
                            gameController.update();
                            continue;
                        }
                    }


                    // klik na deck
                    if (board.getDeckBounds().contains(sf::Vector2f((float)mouseX, (float)mouseY))) {
                        gameController.drawCardForCurrentPlayer(*activeHand, window.getSize().x, window.getSize().y);
                        continue;
                    }

                    // klik na hand provera
                    auto maybe = activeHand->handleClick((float)mouseX, (float)mouseY);
                    if (maybe)
                    {
                        selectedCard = maybe;
                        selectedBoardCard = nullptr;
                        continue;
                    }




                    //    CLICK NA TILE HANDLER I OSTALO
                    int tile = board.getTileIndexAt(mouseX, mouseY);
                    if (tile >= 0) {
                        auto boardCard = board.getCardAt(tile);
                        if (boardCard) {
                            selectedBoardCard = boardCard;
                            selectedCard = nullptr;
                            continue;
                        }

                        if (selectedCard) {
                            const sf::Texture* texPtr = &selectedCard->getSprite().getTexture();
                            sf::Texture* texNonConst = nullptr;

                            if (texPtr) {
                                // const_cast je bezbedan *samo* ako ta texture zaista živi negde (npr. u main-u)
                                texNonConst = const_cast<sf::Texture*>(texPtr);
                            } else {
                                // fallback na neutralnu teksturu ako iz nekog razloga nema texture
                                //texNonConst = &getNeutralTexture();
                                std::cout << "Nema teksture\n";
                            }


                            // napravi CardBoard koristeći postojeći konstruktor (nasleđeni iz Card)
                            auto cb = std::make_shared<CardBoard>(
                                selectedCard->getName(),
                                *texNonConst,
                                selectedCard->getHP(),
                                selectedCard->getDamage(),
                                selectedCard->getCost(),
                                selectedCard->getRarity(),
                                selectedCard->getAttackType(),
                                selectedCard->getDescription()
                            );
                            cb->setOwner(gameController.getCurrentPlayer());
                            // stavi ownera u cardboard trenutni


                            //  postavljanje karte na board
                            bool ok = board.placeCardAt(tile, cb);
                            if (ok) {
                                activeHand->removeHand(selectedCard, window.getSize().x);
                                //cardView.show(*cb);
                                selectedCard = nullptr;
                                std::cout << "Postavio si kartu na board:" << cb->getName() << std::endl;
                            }
                            else
                                std::cout << "Tile vec na sebi ima kartu\n";

                            continue;
                        }
                    }



                    // ===== HAND → GRAVEYARD =====
                    if (board.getGraveyardBounds().contains(sf::Vector2f((float)mouseX, (float)mouseY)))
                    {
                        if (selectedCard)
                        {
                            std::string name = selectedCard->getName();

                            gameController.sendHandCardToGraveyard(*activeHand, graveyard, selectedCard, window.getSize().x);

                            std::cout << "Poslata je karta iz handa na graveyard: "
                                      << name << std::endl;

                            continue;
                        }



                        // ===== BOARD → GRAVEYARD =====
                        if (selectedBoardCard)
                        {
                            std::string name = selectedBoardCard->getName();

                            gameController.sendBoardCardToGraveyard(
                                board,
                                graveyard,
                                selectedBoardCard
                            );

                            std::cout << "Poslata je karta sa boarda na graveyard: "
                                      << name << std::endl;

                            continue;
                        }
                    }


                    selectedCard = nullptr;
                    selectedBoardCard = nullptr;
                    board.handleClick(e->position.x, e->position.y);
                }

                if (e->button == sf::Mouse::Button::Right)
                {
                    if (selectedCard)
                        cardView.show(*selectedCard);
                    else if (selectedBoardCard)
                        cardView.show(*selectedBoardCard);
                    else
                        cardView.hide();
                }


            }
        }

        window.clear(Color::White);
        board.draw(window);
        board.drawBoardCards(window, selectedBoardCard);
        inactiveHand->draw(window, nullptr);   // nema selekcije
        activeHand->draw(window, selectedCard);
        gameDeck.draw(window);


        // crtanje card viewa
        cardView.draw(window);
        turnButtons.draw(window);



        //window.draw(testCard.getSprite());
        window.display();

    }
    return 0;
}