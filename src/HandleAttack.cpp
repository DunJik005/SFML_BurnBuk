#include "HandleAttack.h"


// PRIORITET AKTIVACIJE REDOVA (od najvišeg ka najnižem)
const std::vector<std::vector<int>> attackPhases = {
    {4},        // srednji red
    {3, 5},     // front
    {2, 6},     // middle
    {1, 7},     // back
    {0, 8}      // back-back
};


#include "HandleAttack.h"

void HandleAttack::executeAttacks()
{
    // kolone: desno → levo
    for (int col = Board::COLS - 1; col >= 0; --col)
    {
        for (const auto& phase : attackPhases)
        {
            // 1️⃣ SVI NAPADI U FAZI
            for (int row : phase)
            {
                if (!board.isValidPosition(row, col))
                    continue;

                Tile& tile = board.getTile(row, col);

                if (!tile.isActive() || tile.empty())
                    continue;

                AttackSystem::resolveAttack(board, row, col);
            }

            // 2️⃣ CLEANUP POSLE CELE FAZE
            board.cleanupDeadCards();
        }
    }

    board.incrementAllCardAges();
}
