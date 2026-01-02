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

    boardCards.resize(rows * cols); // svi nullptr
}




void Board::onResize(float windowWidth, float windowHeight)
{
    winW = windowWidth;
    winH = windowHeight;

    recalcLayout();
    windowBackground.setSize({ winW, winH });

    // 🔥 refit svih karata na boardu
    for (int i = 0; i < (int)boardCards.size(); ++i)
    {
        if (boardCards[i])
            fitCardToTile(boardCards[i], i);
    }
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

bool Board::isValidIndex(int index) const {
    return index >= 0 && index < rows * cols;
}

bool Board::hasCardAt(int index) const {
    if (!isValidIndex(index)) return false;
    return boardCards[index] != nullptr;
}


void Board::damagePlayer(Owner attacker, int damage) {
    std::cout
        << "[PLAYER HIT] attacker="
        << (attacker == Owner::Player1 ? "Player1" : "Player2")
        << " damage=" << damage
        << "\n";

    if (attacker == Owner::Player1) {
        // Player1 napada → strada Player2
        player2.takeDamage(damage);
        std::cout
            << "  -> Player2 HP = "
            << player2.getHP()
            << "\n";
    } else {
        // Player2 napada → strada Player1
        player1.takeDamage(damage);
        std::cout
            << "  -> Player1 HP = "
            << player1.getHP()
            << "\n";
    }
}

bool Board::isTopSide(int index) const {
}


// get indexa celije u boardu
int Board::getTileIndexAt(int x, int y)
{
    for (int i = 0; i < (int)tiles.size(); ++i) {
        if (tiles[i].getGlobalBounds().contains(sf::Vector2f((float)x, (float)y)))
            return i;
    }
    return -1;
}





// postavi kartu na polje koje postoji i vec nema kartu
bool Board::placeCardAt(int tileIndex, std::shared_ptr<CardBoard> card)
{

    std::cout
    << "[PLACE TRY] tile=" << tileIndex
    << " cardOwner=" << (card->getOwner() == Owner::Player1 ? "Player1" : "Player2")
    << " tileOwner=" << (getTileOwner(tileIndex) == Owner::Player1 ? "Player1" : "Player2")
    << " isLeech="
    << (card->getAttackType() == AttackType::Pijavica ||
        card->getAttackType() == AttackType::Pijavica_Special ? "YES" : "NO")
    << "\n";



    std::cout
 << "[DEBUG] tile=" << tileIndex
 << " tileOwner=" << (getTileOwner(tileIndex) == Owner::Player1 ? "P1" : "P2")
 << " cardOwner=" << (card->getOwner() == Owner::Player1 ? "P1" :
                      card->getOwner() == Owner::Player2 ? "P2" : "NONE")
 << "\n";




    if (tileIndex < 0 || tileIndex >= (int)boardCards.size())
        return false;
    if (boardCards[tileIndex])
        return false;

    Owner tileOwner = getTileOwner(tileIndex);
    Owner cardOwner = card->getOwner();

    bool isLeech = card->getAttackType() == AttackType::Pijavica || card->getAttackType() == AttackType::Pijavica_Special;

    bool isJelepeno = card->getAttackType() == AttackType::Jelepeno;


    if (!isJelepeno)
    {
        if (!isLeech && tileOwner != cardOwner) {
            std::cout << "Ne mozes staviti kartu na protivnicko polje!\n";
            return false;
        }
        if (isLeech && tileOwner == cardOwner) {
            std::cout << "Ne mozes da stavis pijavicu na svoje tile-ove.\n";
            return false;
        }
    }

    card->tileIndex = tileIndex;
    boardCards[tileIndex] = card;

    fitCardToTile(card, tileIndex);
    return true;
}






Owner Board::getTileOwner(int tileIndex) const {
    int half = boardCards.size() / 2;
    return (tileIndex < half) ? Owner::Player1 : Owner::Player2;
}




void Board::fitCardToTile(const std::shared_ptr<CardBoard>& card, int tileIndex)
{
    if (!card) return;

    const auto& tile = tiles[tileIndex];

    // pozicija
    card->setSpritePosition(
        tile.getPosition().x,
        tile.getPosition().y
    );

    // scale
    sf::Sprite& sprite = card->getSprite();
    sf::FloatRect bounds = sprite.getLocalBounds();

    sprite.setScale({
        tile.getSize().x / bounds.size.x,
        tile.getSize().y / bounds.size.y
    });
}



// nacrtaj karte na boardu
void Board::drawBoardCards(sf::RenderWindow& window,
                           const std::shared_ptr<CardBoard>& selected)
{
    for (auto& cb : boardCards)
    {
        if (!cb) continue;

        // 1️⃣ reset
        cb->resetVisuals();

        // 2️⃣ ako je selektovana → potamni
        if (cb == selected)
        {
            cb->setBrightness(false); // potamni
        }

        // 3️⃣ draw
        cb->draw(window);

        // 4️⃣ dodatni vizuel (border)
        if (cb == selected)
        {
            sf::FloatRect b = cb->getBounds();
            sf::RectangleShape r;
            r.setPosition({b.position.x - 3.f, b.position.y - 3.f});
            r.setSize({b.size.x + 6.f, b.size.y + 6.f});
            r.setFillColor(sf::Color::Transparent);
            r.setOutlineThickness(3.f);
            r.setOutlineColor(sf::Color::Green);
            window.draw(r);
        }
    }
}





std::shared_ptr<CardBoard> Board::getCardAt(int tileIndex) {
    if (tileIndex < 0 || tileIndex >= boardCards.size()) return nullptr;
    return boardCards[tileIndex];
}




std::shared_ptr<CardBoard> Board::removeCardAt(int tileIndex)
{
    if (tileIndex < 0 || tileIndex >= (int)boardCards.size())
        return nullptr;

    auto card = boardCards[tileIndex];
    boardCards[tileIndex].reset();

    return card;
}

void Board::cleanupDeadCards() {
    for (int i = 0; i < (int)boardCards.size(); ++i) {
        auto& card = boardCards[i];
        if (card && card->getHP() <= 0) {

            card.reset();
        }
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