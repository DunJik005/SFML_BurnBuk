#include "AttackSystem.h"


void AttackSystem::resolveAttack(Board& board, int attackerIndex) {
    auto attackerPtr = board.getCardAt(attackerIndex);
    if (!attackerPtr) return;
    if (attackerPtr->getHP() <= 0) return;

    Card& attacker = *attackerPtr;

    std::cout
        << "[ATTACK] Card at tile " << attackerIndex
        << " | HP=" << attacker.getHP()
        << " | DMG=" << attacker.getDamage()
        << " | BASE=" << (int) attacker.getBaseAttack()
        << " | MODS=" << (uint32_t) attacker.getModifiers()
        //<< " | Type=" << (int)attacker.getAttackType()
        << "\n";



    switch (attacker.getBaseAttack()) {
        case BaseAttack::Chomper:
            linearAttack(board, attacker, attackerIndex);
            break;

        case BaseAttack::Peashooter:
            linearAttack(board, attacker, attackerIndex);
            break;

        case BaseAttack::Catapult:
            linearAttack(board, attacker, attackerIndex);
            break;

        case BaseAttack::Nut:
            attackNut(board, attackerIndex);
            break;

        case BaseAttack::Jelepeno:
            attackJelepeno(board, attackerIndex);
            break;

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




void AttackSystem::linearAttack(Board& board, Card& attacker, int index) {
    int dir   = getDirection(attacker);
    int range = getRange(attacker.getBaseAttack());

    bool laser   = hasModifier(attacker.getModifiers(), AttackModifier::Laser);
    bool rapid   = hasModifier(attacker.getModifiers(), AttackModifier::Rapid);
    bool wide    = hasModifier(attacker.getModifiers(), AttackModifier::Wide);
    bool reverse = hasModifier(attacker.getModifiers(), AttackModifier::Reverse);
    bool zlatni  = hasModifier(attacker.getModifiers(), AttackModifier::Zlatni);

    int hits = rapid ? attacker.getHitCount() : 1;
    const int cols = 4;

    // kolone koje se gadjaju
    std::vector<int> columnOffsets = { 0 };
    if (wide) {
        columnOffsets.push_back(-1);
        columnOffsets.push_back(+1);
    }

    for (int h = 0; h < hits; h++) {

        for (int colOffset : columnOffsets) {

            int baseCol = index % cols;
            int newCol  = baseCol + colOffset;
            if (newCol < 0 || newCol >= cols)
                continue;

            // =============================
            // 1. sakupi sve tile-ove u range-u
            // =============================
            std::vector<int> tilesInRange;
            for (int step = 1; step <= range; step++) {
                int t = index + dir * step + colOffset;
                if (board.isValidIndex(t))
                    tilesInRange.push_back(t);
            }

            if (reverse)
                std::reverse(tilesInRange.begin(), tilesInRange.end());

            // =============================
            // 2. traži PRVU kartu
            // =============================
            bool hitCard = false;

            for (int t : tilesInRange) {
                auto target = board.getCardAt(t);
                if (!target) {
                    if (laser) {
                        // laser udara playera i nastavlja
                        std::cout << "    No card at " << t << " -> hits PLAYER (laser)\n";
                        board.damagePlayer(attacker.getOwner(), attacker.getDamage());
                        continue; // laser nastavlja dalje kroz range
                    } else {
                        // običan napad samo preskače tile
                        continue;
                    }
                }


                int hpBefore = target->getHP();
                target->setHP(hpBefore - attacker.getDamage());

                std::cout
                    << "    Hit CARD at " << t
                    << " HP " << hpBefore
                    << " -> " << target->getHP() << "\n";

                if (zlatni && target->getHP() < 0)
                {
                    int excess = -target->getHP();
                    board.damagePlayer(attacker.getOwner(), excess);
                    target->setHP(0);
                }

                hitCard = true;
                if (!laser)
                    break; // karta ima prioritet
            }

            // =============================
            // 3. ako NIJEDNA karta -> player
            // =============================
            if (!hitCard)
            {
                std::cout
                    << "    No cards in column "<< newCol << "-> hits PLAYER\n";
                board.damagePlayer(attacker.getOwner(), attacker.getDamage());
            }
        }
    }
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