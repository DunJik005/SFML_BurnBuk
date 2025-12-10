#ifndef SFMLPROJECT_BOARD_H
#define SFMLPROJECT_BOARD_H

#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Board {
public:
    Board(float windowWidth, float windowHeight);

    void onResize(float windowWidth, float windowHeight);
    void draw(sf::RenderWindow& window);
    void handleClick(int x, int y);

private:
    // Procenti – ti ovde menjaš kako želiš
    float tileW = 0.12f;
    float tileH = tileW*6/5;
    float spacing = 0.01f;
    float sideMargin = 0.02f;
    float topMargin = 0.03f;

    int rows = 4;
    int cols = 4;

    // dinamički računato
    float winW;
    float winH;

    sf::RectangleShape graveyard;
    sf::RectangleShape deck;

    std::vector<sf::RectangleShape> tiles;

    void recalcLayout();
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


