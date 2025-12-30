#include "AttackSystem.h"


void AttackSystem::resolveAttack(Board& board, int attackerIndex) {
    auto attacker = board.getCardAt(attackerIndex);
    if (!attacker) return;
    if (attacker->getHP() <= 0) return;

    std::cout
        << "[ATTACK] Card at tile "
        << attackerIndex
        << " | HP=" << attacker->getHP()
        << " | DMG=" << attacker->getDamage()
        << " | Type=" << (int)attacker->getAttackType()
        << "\n";

    switch (attacker->getAttackType()) {
        case AttackType::Chomper:
            attackChomper(board, attackerIndex);
            break;

        case AttackType::Peashooter:
            attackPeashooter(board, attackerIndex);
            break;

        case AttackType::Nut:
            attackNut(board, attackerIndex);
            break;
        case AttackType::Jelepeno:
            attackJelepeno(board, attackerIndex);

        default:
            break;
    }
}



int AttackSystem::getDirection(const Card& card) {
    if (card.getOwner() == Owner::Player1)
        return +4;
    return -4;
}



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
}