#ifndef SFMLPROJECT_BOARD_H
#define SFMLPROJECT_BOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include "CardBoard.h"
#include "CardHand.h"
#include "Deck.h"
#include "Player.h"

class Board {
public:
    Board(float windowWidth, float windowHeight);

    void onResize(float windowWidth, float windowHeight);
    void draw(sf::RenderWindow& window);
    void handleClick(int x, int y);




    //combat

    // -------- COMBAT API --------

    bool isValidIndex(int index) const;
    bool hasCardAt(int index) const;

    std::shared_ptr<CardBoard> getCardAt(int tileIndex);
    std::shared_ptr<CardBoard> removeCardAt(int tileIndex);

    void cleanupDeadCards();

    // damage igracu na osnovu pozicije napada
    void damagePlayer(Owner attacker, int dmg);

    // strana boarda
    bool isTopSide(int index) const;





    // u Board.h (public)
    int getTileIndexAt(int x, int y);
    bool placeCardAt(int tileIndex, std::shared_ptr<CardBoard> card);
    void drawBoardCards(sf::RenderWindow& window,
                    const std::shared_ptr<CardBoard>& selected);


    // za deck kockicu
    sf::FloatRect getDeckBounds() const { return deck.getGlobalBounds(); }
    sf::Vector2f getDeckPosition() const { return deck.getPosition(); }


    sf::FloatRect getGraveyardBounds() const { return graveyard.getGlobalBounds(); }

    Player& getPlayer1() { return player1; }
    Player& getPlayer2() { return player2; }




private:
    // Procenti – ti ovde menjaš kako želiš
    float tileW = 0.12f;
    float tileH = tileW * 6 / 5;
    float spacing = 0.01f;
    float sideMargin = 0.02f;
    float topMargin = 0.03f;

    int rows = 4;
    int cols = 4;

    // dinamički računato
    float winW;
    float winH;

    sf::RectangleShape graveyard;
    //Deck* deck;
    sf::RectangleShape deck;

    std::vector<sf::RectangleShape> tiles;

    void recalcLayout();
    void fitCardToTile(const std::shared_ptr<CardBoard>& card, int tileIndex);

    Owner getTileOwner(int tileIndex)const;


    // u Board.h (privatni deo)
    //std::vector<std::shared_ptr<CardHand>> hand;
    std::vector<std::shared_ptr<CardBoard>> boardCards; // size = rows*cols, nullptr ako prazno

    // dodatno
    std::shared_ptr<CardHand> selectedHandCard = nullptr;


    Player player1;
    Player player2;

};

#endif

/*class Card; // forward declaration

class Board {
private:
    static constexpr int rows = 4;
    static constexpr int cols = 4;

    static constexpr float tileWidth = 200.f;
    static constexpr float tileHeight = 250.f;
    static constexpr float tileSpacing = 10.f;

    static constexpr float topMargin = 25.f;
    static constexpr float sideMargin = 25.f; // leva i desna margina

    std::vector<sf::RectangleShape> tiles;

    sf::Texture windowBackgroundTexture;
    sf::RectangleShape windowBackground; // pozadina celog prozora

    // Deck i Graveyard placeholders
    sf::RectangleShape deck;
    sf::RectangleShape graveyard;

    sf::Texture tileTexture; // jedna tekstura za sve tile-ove

public:
    Board();
    void draw(sf::RenderWindow& window);
    void handleClick(int mouseX, int mouseY);
};

#endif // SFMLPROJECT_BOARD_H*/