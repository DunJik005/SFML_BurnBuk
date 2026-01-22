#include "Board.h"
#include "Tile.h"
#include <iostream>

// ---------- Ctor ----------
Board::Board(float windowWidth, float windowHeight)
    : winW(windowWidth), winH(windowHeight)
{

    if (!windowBackgroundTexture.loadFromFile("assets/backgroundtexture.png")) {
        std::cerr << "Ne mogu da ucitam backgroundtexture.png!\n";
    }

    windowBackground.setSize({winW, winH});
    windowBackground.setPosition({0.f, 0.f});
    windowBackground.setTexture(&windowBackgroundTexture);

    if (!tileActiveTex.loadFromFile("assets/tiletexture.png")) {
        std::cerr << "Ne mogu da ucitam tiletexture.png!\n";
    }

    initTiles();
    recalcLayout();
}

// ---------- Init tiles ----------
void Board::initTiles()
{
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {

            Tile& tile = grid[r][c];

            // Active / Inactive rows
            if (r == 1 || r == 3 || r == 5 || r == 7)
                tile.setState(Tile::State::Active);
            else
                tile.setState(Tile::State::Inactive);

            if (tile.isActive())
                tile.setTexture(&tileActiveTex);

            // Ownership
            if (r < Board::ROWS / 2)          // 0,1,2,3
                tile.setOwner(Owner::Player1);
            else if (r > Board::ROWS / 2)     // 5,6,7,8
                tile.setOwner(Owner::Player2);
            else                              // r == 4
                tile.setOwner(Owner::None);

        }
    }
}

// ---------- Resize ----------
void Board::onResize(float windowWidth, float windowHeight)
{

    winW = windowWidth;
    winH = windowHeight;
    windowBackground.setSize({ winW, winH });

    recalcLayout();
}

// ---------- Layout ----------
void Board::recalcLayout()
{
    const float pxTileW   = winW * tileW;
    const float pxTileH   = pxTileW * 6.f / 5.f;
    const float pxSpacing = winW * spacing;

    const float boardWidth =
        COLS * pxTileW + (COLS - 1) * pxSpacing;

    const float startX = (winW - boardWidth) / 2.f;

    // 1️⃣ definisi FRONT redove (sidra)
    const int p1FrontRow = 3;
    const int p2FrontRow = 5;

    // 2️⃣ Y pozicija front linije (CENTAR EKRANA)
    const float centerY = winH / 2.f;

    // 3️⃣ Postavi Player1 front red
    for (int c = 0; c < COLS; c++) {
        grid[p1FrontRow][c].setSize(pxTileW, pxTileH);
        grid[p1FrontRow][c].setPosition(
            startX + c * (pxTileW + pxSpacing),
            centerY - pxTileH - pxSpacing / 2.f
        );
    }

    // 4️⃣ Postavi Player2 front red
    for (int c = 0; c < COLS; c++) {
        grid[p2FrontRow][c].setSize(pxTileW, pxTileH);
        grid[p2FrontRow][c].setPosition(
            startX + c * (pxTileW + pxSpacing),
            centerY + pxSpacing / 2.f
        );
    }

    // 5️⃣ Player1 – redovi IZA fronta (naviše)
    int offset = 1;
    for (int r = p1FrontRow - 1; r >= 0; r--) {
        if (!grid[r][0].isActive()) continue;

        for (int c = 0; c < COLS; c++) {
            grid[r][c].setSize(pxTileW, pxTileH);
            grid[r][c].setPosition(
                startX + c * (pxTileW + pxSpacing),
                centerY - (offset + 1) * (pxTileH + pxSpacing)
            );
        }
        offset++;
    }

    // 6️⃣ Player2 – redovi IZA fronta (naniže)
    offset = 1;
    for (int r = p2FrontRow + 1; r < ROWS; r++) {
        if (!grid[r][0].isActive()) continue;

        for (int c = 0; c < COLS; c++) {
            grid[r][c].setSize(pxTileW, pxTileH);
            grid[r][c].setPosition(
                startX + c * (pxTileW + pxSpacing),
                centerY + offset * (pxTileH + pxSpacing)
            );
        }
        offset++;
    }
}

std::pair<int, int> Board::getTileAtPosition(float x, float y) const
{
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {

            const Tile& tile = grid[r][c];

            // 1️⃣ inactive tile-ovi se IGNORIŠU
            if (!tile.isActive())
                continue;

            // 2️⃣ samo vidljivi tile-ovi ulaze u hit-test
            if (tile.getBounds().contains({x, y}))
                return { r, c };
        }
    }

    return { -1, -1 }; // ništa kliknuto
}

void Board::cleanupDeadCards() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            grid[r][c].cleanupDeadCards();
}

bool Board::handleClick(int mouseX, int mouseY)
{
    auto [row, col] = getTileAtPosition(
        static_cast<float>(mouseX),
        static_cast<float>(mouseY)
    );

    if (row == -1)
        return false;

    std::cout << "Kliknut tile: (" << row << ", " << col << ")\n";
    return true;
}


void Board::updateHover(int mouseX, int mouseY)
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Tile& tile = grid[r][c];

            if (!tile.isActive())
            {
                tile.setHovered(false);
                continue;
            }

            bool isHover =
                tile.getBounds().contains(
                    {static_cast<float>(mouseX),
                    static_cast<float>(mouseY)}
                );

            tile.setHovered(isHover);
        }
    }
}


bool Board::placeCard(int row, int col, std::shared_ptr<Card> card)
{
    if (!card)
        return false;

    if (!isValidPosition(row, col))
        return false;

    Tile& tile = grid[row][col];

    // debug (opciono, ali korisno)
    std::cout
        << "[PLACE TRY] row=" << row
        << " col=" << col
        << " cardOwner="
        << (card->getOwner() == Owner::Player1 ? "P1" :
            card->getOwner() == Owner::Player2 ? "P2" : "NONE")
        << " tileOwner="
        << (tile.getOwner() == Owner::Player1 ? "P1" :
            tile.getOwner() == Owner::Player2 ? "P2" : "NONE")
        << "\n";

    if (!tile.placeCard(card))
    {
        std::cout << "❌ Tile rejected card placement\n";
        return false;
    }

    return true;
}


std::shared_ptr<Card> Board::removeTopCard(int row, int col) {
    if (!isValidPosition(row, col))
        return nullptr;

    return grid[row][col].removeTopCard();
}

int Board::getNextActiveRow(int fromRow, Owner attacker) const
{
    int step = (attacker == Owner::Player1) ? 1 : -1;
    int r = fromRow + step;

    while (r >= 0 && r < ROWS) {
        if (grid[r][0].isActive())
            return r;
        r += step;
    }

    return -1;
}
int Board::getLeftColumn(int col) const
{
    if (col <= 0)
        return -1;
    return col - 1;
}

int Board::getRightColumn(int col) const
{
    if (col >= COLS - 1)
        return -1;
    return col + 1;
}

Tile* Board::getNextTile(int row, int col)
{
    if (!isValidPosition(row, col))
        return nullptr;

    Owner tileOwner = grid[row][col].getOwner();

    int dir = 0;
    if (tileOwner == Owner::Player1)
        dir = +1;
    else if (tileOwner == Owner::Player2)
        dir = -1;
    else
        return nullptr;

    int nextRow = row + dir;

    if (!isValidPosition(nextRow, col))
        return nullptr;

    return &grid[nextRow][col];
}

Tile* Board::getNextTile(int row, int col, Owner directionOwner)
{
    if (!isValidPosition(row, col))
        return nullptr;

    int dir = 0;
    if (directionOwner == Owner::Player1)
        dir = +1;
    else if (directionOwner == Owner::Player2)
        dir = -1;
    else
        return nullptr;

    int nextRow = row + dir;

    if (!isValidPosition(nextRow, col))
        return nullptr;

    return &grid[nextRow][col];
}


std::vector<std::shared_ptr<Card>>
Board::getCardsInRangeFrom(
    int startRow,
    int startCol,
    int range,
    bool enemyOnly
)
{
    std::vector<std::shared_ptr<Card>> result;

    if (!isValidPosition(startRow, startCol))
        return result;

    Owner originOwner = grid[startRow][startCol].getOwner();

    int dir = 0;
    if (originOwner == Owner::Player1)
        dir = +1;
    else if (originOwner == Owner::Player2)
        dir = -1;
    else
        return result;

    for (int step = 1; step <= range; ++step)
    {
        int r = startRow + step * dir;
        int c = startCol;

        if (!isValidPosition(r, c))
            break;

        Tile& tile = grid[r][c];

        if (!tile.isActive())
            continue;

        if (tile.empty())
            continue;

        if (enemyOnly && tile.getOwner() == originOwner)
            continue;

        auto target = tile.getAttackTarget();
        if (target)
            result.push_back(target);
    }

    return result;
}

bool Board::getTargetPosition(
    int row,
    int col,
    Direction dir,
    int& outRow,
    int& outCol
) const
{
    if (!isValidPosition(row, col))
        return false;

    outRow = row;
    outCol = col;

    Owner owner = grid[row][col].getOwner();

    switch (dir)
    {
        case Direction::Forward:
            if (owner == Owner::Player1) outRow++;
            else if (owner == Owner::Player2) outRow--;
            else return false;
            break;

        case Direction::Backward:
            if (owner == Owner::Player1) outRow--;
            else if (owner == Owner::Player2) outRow++;
            else return false;
            break;

        case Direction::Left:
            outCol--;
            break;

        case Direction::Right:
            outCol++;
            break;
    }

    return isValidPosition(outRow, outCol);
}




// ---------- Draw ----------
void Board::draw(sf::RenderWindow& window)
{
    window.draw(windowBackground);
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            grid[r][c].draw(window);
}









sf::Vector2f Board::getDeckPosition() const {
    // desno od boarda, u visini prvog reda
    const Tile& t = grid[0][COLS - 1];
    auto pos = t.getPosition();
    return { pos.x + t.getSize().x + winW * 0.02f, pos.y };
}



sf::FloatRect Board::getDeckBounds() const {
    auto pos = getDeckPosition();
    float w = winW * 0.08f;
    float h = w * 1.4f;

    return sf::FloatRect(
        sf::Vector2f(pos.x, pos.y),
        sf::Vector2f(w, h)
    );
}

sf::FloatRect Board::getGraveyardBounds() const {
    const Tile& t = grid[0][0];
    auto pos = t.getPosition();
    float w = winW * 0.08f;
    float h = w * 1.4f;

    return sf::FloatRect(
        sf::Vector2f(pos.x - w - winW * 0.02f, pos.y),
        sf::Vector2f(w, h)
    );
}






void Board::drawBoardCards(
    sf::RenderWindow& window,
    const std::shared_ptr<Card>& selected)
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Tile& tile = grid[r][c];

            if (!tile.isActive())
                continue;

            auto card = tile.getAttackTarget();
            // ili getTopCard() ako imaš stack

            if (!card)
                continue;

            // 1️⃣ reset visuals
            card->resetVisuals();

            // 2️⃣ selektovana karta → potamni
            if (card == selected)
            {
                card->setBrightness(false);
            }

            // 3️⃣ draw karta
            card->draw(window);

            // 4️⃣ border za selektovanu
            if (card == selected)
            {
                sf::FloatRect b = card->getBounds();

                sf::RectangleShape border;
                border.setPosition(
                    sf::Vector2f(b.position.x - 3.f, b.position.y - 3.f)
                );
                border.setSize(
                    sf::Vector2f(b.size.x + 6.f, b.size.y + 6.f)
                );
                border.setFillColor(sf::Color::Transparent);
                border.setOutlineThickness(3.f);
                border.setOutlineColor(sf::Color::Green);

                window.draw(border);
            }
        }
    }
}



Tile& Board::getTile(int row, int col)
{
    return grid[row][col];
}

const Tile& Board::getTile(int row, int col) const
{
    return grid[row][col];
}



bool Board::isValidPosition(int row, int col) const
{
    return row >= 0 && row < ROWS &&
           col >= 0 && col < COLS;
}




void Board::damagePlayer(Owner attacker, int damage)
{
    if (attacker == Owner::Player1)
        player2.takeDamage(damage);
    else if (attacker == Owner::Player2)
        player1.takeDamage(damage);
}
