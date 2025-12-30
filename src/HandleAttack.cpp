#include "HandleAttack.h"

const std::vector<std::pair<int,int>>HandleAttack::attackPairs = {
    {7, 11},
    {3, 15},
    {6, 10},
    {2, 14},
    {5, 9},
    {1, 13},
    {4, 8},
    {0, 12}
};

void HandleAttack::executeAttacks() {
    for (const auto& [a, b] : attackPairs) {

        auto cardA = board.getCardAt(a);
        auto cardB = board.getCardAt(b);

        bool aAlive = cardA && cardA->getHP() > 0;
        bool bAlive = cardB && cardB->getHP() > 0;

        // Oba napadaju “praktično simultano”
        if (aAlive && bAlive) {
            AttackSystem::resolveAttack(board, a);
            AttackSystem::resolveAttack(board, b);
        }
        // Ako je samo jedna živa → napada samo ona
        else if (aAlive) {
            AttackSystem::resolveAttack(board, a);
        }
        else if (bAlive) {
            AttackSystem::resolveAttack(board, b);
        }

    }

    board.cleanupDeadCards();
}
