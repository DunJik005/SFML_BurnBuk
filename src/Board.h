#ifndef SFMLPROJECT_BOARD_H
#define SFMLPROJECT_BOARD_H
#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Tile.h"
#include "Player.h"

enum class Direction {
    Forward,
    Backward,
    Left,
    Right
};

class Board {
public:
    static constexpr int ROWS = 9;
    static constexpr int COLS = 4;

public:

    Board(float winWidth, float winHeight);

    void onResize(float winWidth, float winHeight);

    bool handleClick(int mouseX, int mouseY);

    void updateHover(int mouseX, int mouseY);


    Tile& getTile(int row, int col);
    const Tile& getTile(int row, int col) const;

    bool isValidPosition(int row, int col) const;

    bool placeCard(int row, int col, std::shared_ptr<Card> card);

    std::shared_ptr<Card> removeTopCard(int row, int col);

    void damagePlayer(Owner attacker, int damage);

    bool isEnemySide(int row, Owner owner) const;

    void draw(sf::RenderWindow& window);

    std::pair<int, int> getTileAtPosition(float x, float y) const;

    int getNextActiveRow(int fromRow, Owner attacker) const;

    int getLeftColumn(int col) const;
    int getRightColumn(int col) const;





    // moje ubaceno za deck graveyard i iscrtavanje
    sf::Vector2f getDeckPosition() const;
    sf::FloatRect getDeckBounds() const;
    sf::FloatRect getGraveyardBounds() const;
    void drawBoardCards(
    sf::RenderWindow& window,
    const std::shared_ptr<Card>& selected
);






    // ---------- Navigation ----------
    Tile* getNextTile(int row, int col);
    Tile* getNextTile(int row, int col, Owner owner);

    Tile* getTileAtOffset(int row, int col, int dRow, int dCol);

    // ---------- Movement ----------
    bool moveTopCard(int row, int col, Direction dir);
    bool moveStack(int row, int col, Direction dir);



    // ---------- Range queries ----------
    std::vector<std::shared_ptr<Card>>
    getCardsInRangeFrom(
        int startRow,
        int startCol,
        int range,
        bool enemyOnly = true
    );

    bool getTargetPosition(
    int row,
    int col,
    Direction dir,
    int& outRow,
    int& outCol
    ) const;


    void cleanupDeadCards();

    Player& getPlayer1();

    Player& getPlayer2();

private:

    std::array<std::array<Tile, COLS>, ROWS> grid;


    // ---------- Window ----------
    float winW;
    float winH;

    sf::Texture windowBackgroundTexture;
    sf::Texture tileActiveTex;

    sf::RectangleShape windowBackground; // pozadina celog prozora

    // ---------- Visual layout ----------
    float tileW = 0.12f;      // u procentima width-a
    float tileH = tileW * 6 / 5;
    float spacing = 0.01f;
    float sideMargin = 0.02f;
    float topMargin = 0.03f;

    Player player1;
    Player player2;

    // ---------- Internal helpers ----------
    void initTiles();     // inicijalni active/inactive + ownership
    void recalcLayout();  // pozicioniranje tile-ova

};
#endif // SFMLPROJECT_BOARD_H*/