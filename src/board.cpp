#include "Board.h"
#include <iostream>

sf::RectangleShape windowBackground;
sf::Texture windowBackgroundTexture;

sf::Texture tileTexture;

Board::Board(float windowWidth, float windowHeight)
{
    winW = windowWidth;
    winH = windowHeight;

    // --- window background ---
    if (!windowBackgroundTexture.loadFromFile("assets/backgroundtexture.png")) {
        std::cerr << "Ne mogu da ucitam backgroundtexture.png!\n";
    }
    windowBackground.setSize({winW, winH});
    windowBackground.setPosition({0.f, 0.f});
    windowBackground.setTexture(&windowBackgroundTexture);

    if (!tileTexture.loadFromFile("assets/tiletexture.png")) {
        std::cerr << "Ne mogu da ucitam tiletexture.png!\n";
    }

    // pripremi sve tileove
    tiles.resize(rows * cols);
    for (auto& t : tiles)
    {
        t.setFillColor(sf::Color(200, 200, 220));
        t.setTexture(&tileTexture); // tile texture
    }

    graveyard.setFillColor(sf::Color(180, 50, 50));
    deck.setFillColor(sf::Color(50, 120, 180));

    // učitaj tile texture
    if (!tileTexture.loadFromFile("assets/tiletexture.png")) {
        std::cerr << "Ne mogu da ucitam tiletexture.png!\n";
    }

    recalcLayout();
}

void Board::onResize(float windowWidth, float windowHeight)
{
    winW = windowWidth;
    winH = windowHeight;
    recalcLayout();

    windowBackground.setSize({ winW, winH });
}

void Board::recalcLayout()
{
    float pxTileW   = winW * tileW;
    float pxTileH   = pxTileW * 6.f / 5.f;
    float pxSpacing = winW * spacing;

    float boardWidth  = cols * pxTileW + (cols - 1) * pxSpacing;
    float boardHeight = rows * pxTileH + (rows - 1) * pxSpacing;

    float offsetX = (winW - (boardWidth + pxTileW*2 + pxSpacing*3)) / 2.f;
    float offsetY = (winH - boardHeight) / 2.f;

    // start x za table (posle graveyarda)
    float boardStartX = offsetX + pxTileW + pxSpacing;
    float boardStartY = offsetY;

    // postavi tileove
    for (int r = 0; r < rows; r++)
    {
        for (int c = 0; c < cols; c++)
        {
            int idx = r * cols + c;

            float x = boardStartX + c * (pxTileW + pxSpacing);
            float y = boardStartY + r * (pxTileH + pxSpacing);

            tiles[idx].setSize({pxTileW, pxTileH});
            tiles[idx].setPosition({x, y});
        }
    }

    // graveyard levo
    graveyard.setSize({pxTileW, pxTileH});
    graveyard.setPosition({offsetX, boardStartY});

    // deck desno
    float deckX = boardStartX + boardWidth + pxSpacing;
    deck.setSize({pxTileW, pxTileH});
    deck.setPosition({deckX, boardStartY});
}

void Board::draw(sf::RenderWindow& window)
{
    window.draw(windowBackground);
    window.draw(graveyard);
    window.draw(deck);

    for (auto& t : tiles)
        window.draw(t);
}

void Board::handleClick(int x, int y)
{
    // primer koji tile se kliknuo
    for (int i = 0; i < tiles.size(); i++)
    {
        if (tiles[i].getGlobalBounds().contains(sf::Vector2f(x, y)))
            std::cout << "Kliknut tile " << i << "\n";
    }
}



/*
#include "Board.h"
#include <iostream>

Board::Board() {
    // --- window background ---
    if (!windowBackgroundTexture.loadFromFile("assets/backgroundtexture.png")) {
        std::cerr << "Ne mogu da ucitam window_background.png!\n";
    }
    windowBackground.setSize({1320.f, 1080.f});
    windowBackground.setPosition({0.f, 0.f});
    windowBackground.setTexture(&windowBackgroundTexture);

    // --- tile texture ---
    if (!tileTexture.loadFromFile("assets/tiletexture.png")) {
        std::cerr << "Ne mogu da ucitam tiletexture.png!\n";
    }

    // Board pozicija
    float boardX = sideMargin + tileWidth + tileSpacing; // levo od board-a je graveyard
    float boardY = topMargin;

    tiles.reserve(rows * cols);

    // Kreiranje board tiles
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            sf::RectangleShape tile;
            tile.setSize({ tileWidth, tileHeight });

            float x = boardX + c * (tileWidth + tileSpacing);
            float y = boardY + r * (tileHeight + tileSpacing);
            tile.setPosition({ x, y });

            tile.setTexture(&tileTexture); // postavljamo istu sliku za sve

            tiles.push_back(tile);
        }
    }

    // Graveyard (levo)
    graveyard.setSize({ tileWidth, tileHeight });
    graveyard.setFillColor(sf::Color(100, 100, 100));
    graveyard.setPosition({ sideMargin, boardY });

    // Deck (desno)
    deck.setSize({ tileWidth, tileHeight });
    deck.setFillColor(sf::Color(150, 150, 150));
    deck.setPosition({ boardX + cols * tileWidth + (cols - 1) * tileSpacing + tileSpacing, boardY });
}

void Board::draw(sf::RenderWindow& window) {
    // prvo pozadina celog prozora
    window.draw(windowBackground);

    // potom tiles
    for (auto& t : tiles)
        window.draw(t);

    // potom deck i graveyard
    window.draw(deck);
    window.draw(graveyard);
}

void Board::handleClick(int mouseX, int mouseY) {
    float boardX = sideMargin + tileWidth + tileSpacing;
    float boardY = topMargin;

    mouseX -= boardX;
    mouseY -= boardY;

    if (mouseX < 0 || mouseY < 0) return;

    int col = mouseX / (tileWidth + tileSpacing);
    int row = mouseY / (tileHeight + tileSpacing);

    if (col >= cols || row >= rows) return;

    int index = row * cols + col;
    std::cout << "Selected tile: " << index << std::endl;

    for (int i = 0; i < tiles.size(); i++) {
        if (i == index)
            tiles[i].setFillColor(sf::Color(100, 100, 255));
        else {
            if (i / cols < 2)
                tiles[i].setFillColor(sf::Color(255, 0, 0)); // samo vizuelni test
            else
                tiles[i].setFillColor(sf::Color(0, 255, 0));
        }
    }
}
*/
