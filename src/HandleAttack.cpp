#include "HandleAttack.h"


// PRIORITET AKTIVACIJE REDOVA (od najvišeg ka najnižem)
const std::vector<int> attackRows = {
    4, // srednji red (ako je aktivan)
    3, // Player1 front
    5, // Player2 front
    2, // Player1 middle (default inactive)
    6, // Player2 middle (default inactive)
    1, // Player1 back
    7, // Player2 back
    0, // Player1 back-back
    8  // Player2 back-back
};


#include "HandleAttack.h"

void HandleAttack::executeAttacks()
{
    // kolone: desno → levo
    for (int col = Board::COLS - 1; col >= 0; --col)
    {
        for (int row : attackRows)
        {
            if (!board.isValidPosition(row, col))
                continue;

            Tile& tile = board.getTile(row, col);

            if (!tile.isActive())
                continue;

            if (tile.empty())
                continue;

            // 🔥 samo aktiviramo kartu
            AttackSystem::resolveAttack(board, row, col);
        }
    }

    board.cleanupDeadCards();
}