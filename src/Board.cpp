#include "Board.h"
#include <iostream>



static constexpr float BOARD_HEIGHT_RATIO = 0.70f; // 70%


// ---------- Ctor ----------
Board::Board(float windowWidth, float windowHeight, Player& p1, Player& p2)
    : winW(windowWidth), winH(windowHeight), player1(p1), player2(p2)
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
    // 1️⃣ visina boarda (60% ekrana)
    const float boardHeight = winH * BOARD_HEIGHT_RATIO;
    const float boardTop    = (winH - boardHeight) / 2.f;

    // 2️⃣ spacing (horizontalni i vertikalni isti)
    const float pxSpacing = winW * spacing;

    // 3️⃣ pronađi aktivne redove
    std::vector<int> activeRows;
    for (int r = 0; r < ROWS; r++)
    {
        if (grid[r][0].isActive())
            activeRows.push_back(r);
    }

    const int rowCount = static_cast<int>(activeRows.size());
    if (rowCount == 0)
        return;

    // 4️⃣ izračunaj veličinu karte iz VISINE (proporcija 6 : 5)
    const float availableH =
        boardHeight - (rowCount - 1) * pxSpacing;

    const float tileH = availableH / rowCount;
    const float tileW = tileH * 5.f / 6.f;

    // 5️⃣ ukupna širina boarda
    const float boardWidth =
        COLS * tileW + (COLS - 1) * pxSpacing;

    const float startX = (winW - boardWidth) / 2.f;

    // 6️⃣ rasporedi redove odozgo nadole unutar boarda
    float y = boardTop;

    for (int idx = 0; idx < rowCount; idx++)
    {
        int r = activeRows[idx];

        for (int c = 0; c < COLS; c++)
        {
            grid[r][c].setSize(tileW, tileH);
            grid[r][c].setPosition(
                startX + c * (tileW + pxSpacing),
                y
            );
        }

        y += tileH + pxSpacing;
    }
}

float Board::getLeftEdge() const {
    for (int r = 0; r < ROWS; ++r)
    {
        for (int c = 0; c < COLS; ++c)
        {
            const Tile& tile = grid[r][c];
            if (tile.isActive())
                return tile.getPosition().x;
        }
    }
    return 0.f; // fallback
}

float Board::getCenterY() const {
    return winH / 2.f;
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

void Board::incrementAllCardAges()
{
    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Tile& tile = grid[r][c];

            if (!tile.isActive())
                continue;

            const auto& cards = tile.getAllCards();
            for (const auto& card : cards)
            {
                if (card) {
                    card->setAge(card->getAge() + 1);
                    std::cout << card->getName() << "Kartin novi age je: " << card->getAge() << "\n";
                }
            }
        }
    }
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

    if (card->getBaseAttack() == BaseAttack::Jelepeno) {
        std::cout << "Jelepeno bacen na tile i trigerivan\n";
        placeJelepeno(row, col);
    }

    return true;
}





void Board::placeJelepeno(int row, int col)
{
    if (!isValidPosition(row, col))
        return;

    Tile& tile = grid[row][col];

    auto spell = tile.getAttackTarget();
    if (!spell)
        return;

    std::cout
        << "[JELEPENO] on (" << row << "," << col << ") "
        << "DMG=" << spell->getDamage() << "\n";

    // 1. izvrši AoE
    AttackSystem::attackJelepeno(*this, row, col);

    // 2. počisti mrtve karte
    cleanupDeadCards();

    // 3. ukloni spell sa tile-a (ako ga attack nije već uklonio)
    if (!tile.empty() && tile.getAttackTarget() == spell)
        tile.removeTopCard();

    std::cout << "[JELEPENO] removed from board\n";
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

Tile* Board::getNextActiveTileVertical(int row, int col, int dir)
{
    int r = row + dir;

    while (r >= 0 && r < ROWS)
    {
        if (grid[r][col].isActive())
            return &grid[r][col];
        r += dir;
    }
    return nullptr;
}

Tile* Board::getTileAtOffset(int row, int col, int dRow, int dCol)
{
    int newRow = row + dRow;
    int newCol = col + dCol;

    if (newRow < 0 || newRow >= ROWS || newCol < 0 || newCol >= COLS)
        return nullptr;

    return &grid[newRow][newCol];
}

std::vector<const Tile*> Board::getActiveEnemyTiles(Owner attacker) const
{
    std::vector<const Tile*> tiles;

    Owner enemy = (attacker == Owner::Player1 ? Owner::Player2 : Owner::Player1);

    for (int r = 0; r < ROWS; r++)
    {
        // samo redovi koji pripadaju enemy-u
        if (grid[r][0].getOwner() != enemy)
            continue;

        // samo ACTIVE redovi
        if (!grid[r][0].isActive())
            continue;

        for (int c = 0; c < COLS; c++)
        {
            tiles.push_back(&grid[r][c]);
        }
    }

    return tiles;
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




void Board::draw(
sf::RenderWindow& window,
const std::shared_ptr<Card>& selected,
Owner currentPlayer)
{
    window.draw(windowBackground);

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            Tile& tile = grid[r][c];

            if (!tile.isActive())
                continue;

            // 1️⃣ tile
            tile.draw(window);

            // 2️⃣ karta (ako postoji)
            auto card = tile.getAttackTarget();
            if (card)
            {
                card->resetVisuals();

                if (card == selected)
                {
                    sf::FloatRect b = card->getBounds();
                    sf::RectangleShape border;
                    border.setPosition({b.position.x - 3.f, b.position.y - 3.f});
                    border.setSize({b.size.x + 6.f, b.size.y + 6.f});
                    border.setFillColor(sf::Color::Transparent);
                    window.draw(border);
                    if (tile.canReturnCard(currentPlayer) && card)
                    {
                        overlayRenderer.draw(
                            window,
                            card->getBounds(),
                            OverlayType::Return
                        );
                    }
                }
            }
            else if (tile.isHovered())
            {
                overlayRenderer.draw(
                    window,
                    tile.getBounds(),
                    OverlayType::Hover
                );
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

void Board::resetPlacedThisTurn()
{
    for (int r = 0; r < ROWS; ++r)
        for (int c = 0; c < COLS; ++c)
            grid[r][c].setPlacedThisTurnCount(0);
}


void Board::damagePlayer(Owner attacker, int damage)
{
    if (attacker == Owner::Player1)
        player2.takeDamage(damage);
    else if (attacker == Owner::Player2)
        player1.takeDamage(damage);
}