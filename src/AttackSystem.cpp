#include "AttackSystem.h"


void AttackSystem::resolveAttack(Board& board, int row, int col)
{
    if (!board.isValidPosition(row, col))
        return;

    Tile& originTile = board.getTile(row, col);

    auto attackerPtr = originTile.getAttackTarget();
    if (!attackerPtr)
        return;

    if (attackerPtr->getHP() <= 0)
        return;

    Card& attacker = *attackerPtr;

    std::cout
        << "[ATTACK] (" << row << "," << col << ") "
        << "HP=" << attacker.getHP()
        << " DMG=" << attacker.getDamage()
        << " BASE=" << (int)attacker.getBaseAttack()
        << " MODS=" << (uint32_t)attacker.getModifiers()
        << "\n";

    switch (attacker.getBaseAttack())
    {
        case BaseAttack::Chomper:
        case BaseAttack::Peashooter:
        case BaseAttack::Catapult:
        case BaseAttack::Nut:
            linearAttack(board, row, col);
            break;
/*
        case BaseAttack::Nut:
            attackNut(board, row, col);
            break;

        case BaseAttack::Jelepeno:
            attackJelepeno(board, row, col);
            break;
            */

        default:
            break;
    }
}





int AttackSystem::getDirection(const Card& card) {
    return (card.getOwner() == Owner::Player1) ? +4 : -4;
}



int AttackSystem::getRange(BaseAttack base) {
    switch (base) {
        case BaseAttack::Chomper:    return 1;
        case BaseAttack::Peashooter: return 2;
        case BaseAttack::Catapult:   return 3;
        case BaseAttack::Nut:        return 0;
        default:                     return 0;
    }
}



bool AttackSystem::hasModifier(AttackModifier mods, AttackModifier flag) {
    return (static_cast<uint32_t>(mods) & static_cast<uint32_t>(flag)) != 0;
}



void AttackSystem::linearAttack(Board& board, int row, int col)
{
    Tile& originTile = board.getTile(row, col);
    auto attackerPtr = originTile.topCard();
    if (!attackerPtr)
        return;

    Card& attacker = *attackerPtr;

    int range = getRange(attacker.getBaseAttack());

    bool laser   = hasModifier(attacker.getModifiers(), AttackModifier::Laser);
    bool rapid   = hasModifier(attacker.getModifiers(), AttackModifier::Rapid);
    bool wide    = hasModifier(attacker.getModifiers(), AttackModifier::Wide);
    bool reverse = hasModifier(attacker.getModifiers(), AttackModifier::Reverse);
    bool zlatni  = hasModifier(attacker.getModifiers(), AttackModifier::Zlatni);

    int hits = rapid ? attacker.getHitCount() : 1;

    // kolone koje gadjamo
    std::vector<int> cols = { col };
    if (wide)
    {
        int l = board.getLeftColumn(col);
        int r = board.getRightColumn(col);
        if (l != -1)
            cols.push_back(l);
        if (r != -1)
            cols.push_back(r);
    }

    for (int h = 0; h < hits; h++)
    {
        for (int attackCol : cols)
        {
            std::vector<int> rowsInRange;

            int currentRow = row;
            for (int step = 0; step < range; step++)
            {
                currentRow = board.getNextActiveRow(currentRow, attacker.getOwner());
                if (currentRow == -1)
                    break;
                rowsInRange.push_back(currentRow);
            }

            if (reverse)
                std::reverse(rowsInRange.begin(), rowsInRange.end());

            bool hitCard = false;

            for (int r : rowsInRange)
            {
                Tile& tile = board.getTile(r, attackCol);

                if (tile.empty())
                {
                    if (laser)
                        board.damagePlayer(attacker.getOwner(), attacker.getDamage());
                    continue;
                }

                auto target = tile.getAttackTarget();
                if (!target)
                    continue;

                int hpBefore = target->getHP();
                target->setHP(hpBefore - attacker.getDamage());

                std::cout
                    << "    Hit card at (" << r << "," << attackCol
                    << ") HP " << hpBefore << " -> " << target->getHP() << "\n";

                if (zlatni && target->getHP() < 0)
                {
                    board.damagePlayer(attacker.getOwner(), -target->getHP());
                    target->setHP(0);
                }

                hitCard = true;
                if (!laser)
                    break;
            }

            if (!hitCard)
                board.damagePlayer(attacker.getOwner(), attacker.getDamage());
        }
    }
}





























/*


void AttackSystem::attackChomper(Board &board, int index) {

    auto attacker = board.getCardAt(index);
    if (!attacker) return;
    int dir = AttackSystem::getDirection(*attacker);
    int targetIndex = index + dir;

    std::cout
        << "  [CHOMPER] from tile "
        << index
        << " -> checking tile "
        << targetIndex
        << "\n";


    if (!board.isValidIndex(targetIndex)) return;
    auto target = board.getCardAt(targetIndex);
    if (target) {
        std::cout
            << "    Hits CARD at tile "
            << targetIndex
            << " | target HP before=" << target->getHP()
            << "\n";
        target->setHP(target->getHP() - attacker->getDamage());
        std::cout
            << "    Target HP after=" << target->getHP()
            << "\n";
    } else {
        std::cout << "    No card -> hits PLAYER\n";
        board.damagePlayer(attacker->getOwner(), attacker->getDamage());
    }
}

void AttackSystem::attackPeashooter(Board& board, int index) {
    auto attacker = board.getCardAt(index);
    if (!attacker) return;

    int dir = getDirection(*attacker);

    int firstIndex = index + dir;
    int secondIndex = index + dir * 2;

    std::cout
        << "  [PEASHOOTER] from tile "
        << index
        << " checking " << firstIndex << " then " << secondIndex
        << "\n";

    if (board.isValidIndex(firstIndex)) {
        auto target = board.getCardAt(firstIndex);

        if (target) {
            std::cout
                << "    Hits CARD at tile "
                << firstIndex
                << " HP before=" << target->getHP()
                << "\n";
            target->setHP(target->getHP() - attacker->getDamage());

            std::cout
                << "    Target HP after=" << target->getHP()
                << "\n";

            return;
        }
    }
    if (board.isValidIndex(secondIndex)) {
        auto target = board.getCardAt(secondIndex);

        if (target) {
            std::cout
                << "    Hits CARD at tile "
                << secondIndex
                << " HP before=" << target->getHP()
                << "\n";
            target->setHP(target->getHP() - attacker->getDamage());
            std::cout
                << "    Target HP after=" << target->getHP()
                << "\n";
            return;
        }
    }
    board.damagePlayer(attacker->getOwner(), attacker->getDamage());
}

void AttackSystem::attackNut(Board &board, int index) {
    //nista
}

void AttackSystem::attackJelepeno(Board &board, int index) {
    auto attacker = board.getCardAt(index);
    if (!attacker) return;

    const int cols = 4;
    const int rows = 4;

    int row = index / cols;
    int col = index % cols;

    // centralna ćelija + 4 susedne
    std::vector<int> targets;
    targets.push_back(index); // centralna

    if (row > 0) targets.push_back(index - cols); // gore
    if (row < rows - 1) targets.push_back(index + cols); // dole
    if (col > 0) targets.push_back(index - 1); // levo
    if (col < cols - 1) targets.push_back(index + 1); // desno

    std::cout << "  [JELEPENO] AoE from tile " << index << " -> targets:";
    for (int t : targets) std::cout << " " << t;
    std::cout << "\n";

    for (int t : targets) {
        if (!board.isValidIndex(t)) continue;
        auto target = board.getCardAt(t);
        if (target) {
            std::cout << "    Hits CARD at tile " << t
                      << " | HP before=" << target->getHP() << "\n";
            target->setHP(target->getHP() - attacker->getDamage());
            std::cout << "    Target HP after=" << target->getHP() << "\n";
        } else {
            std::cout << "    No card at tile " << t << " -> hits PLAYER\n";
            board.damagePlayer(attacker->getOwner(), attacker->getDamage());
        }
    }

    // karta se odmah uklanja sa boarda
    board.removeCardAt(index);
    std::cout << "  Jelepeno card at tile " << index << " removed after AoE\n";
}*/