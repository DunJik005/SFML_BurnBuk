#include "AttackSystem.h"
#include "Board.h"
#include "Tile.h"
#include "Card.h"


void AttackSystem::resolveAttack(Board& board, int row, int col)
{
    if (!board.isValidPosition(row, col))
        return;

    Tile& originTile = board.getTile(row, col);

    auto attackerPtr = originTile.getAttackTarget();
    if (!attackerPtr)
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

        case BaseAttack::Jelepeno:
            attackJelepeno(board, row, col);
            break;

        case BaseAttack::Hemisfera:
            attackHemisfera(board, row, col);
            break;

        case BaseAttack::Sniper:
            attackSniper(board, row, col);
            break;


        case BaseAttack::Pump:
            attackPump(board, row, col);
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



void AttackSystem::linearAttack(Board& board, int row, int col)
{
    std::cout << "[LINEAR ATTACK] from (" << row << "," << col << ")";

    Tile& originTile = board.getTile(row, col);
    auto attackerPtr = originTile.topCard();
    if (!attackerPtr)
    {
        std::cout << "   no card on origin tile\n";
        return;
    }

    Card& attacker = *attackerPtr;

    int range = getRange(attacker.getBaseAttack());

    bool laser   = hasModifier(attacker.getModifiers(), AttackModifier::Laser);
    bool rapid   = hasModifier(attacker.getModifiers(), AttackModifier::Rapid);
    bool wide    = hasModifier(attacker.getModifiers(), AttackModifier::Wide);
    bool reverse = hasModifier(attacker.getModifiers(), AttackModifier::Reverse);
    bool zlatni  = hasModifier(attacker.getModifiers(), AttackModifier::Zlatni);

    int hits = rapid ? attacker.getHitCount() : 1;



    std::cout
        << "attacker = " << attacker.getName()
        << " owner = " << (int)attacker.getOwner()
        << " dmg=" << attacker.getDamage()
        << " range=" << range
        << " hits=" << hits
        << " laser=" << laser
        << " wide=" << wide
        << " reverse=" << reverse
        << " zlatni=" << zlatni
        << "\n";

    // ======================================
    // Kolone koje napadamo
    // ======================================
    std::vector<int> cols = { col };

    if (wide)
    {
        int l = board.getLeftColumn(col);
        int r = board.getRightColumn(col);

        if (l != -1) cols.push_back(l);
        if (r != -1) cols.push_back(r);
    }

    std::cout << "attack columns:";
    for (int c : cols) std::cout << " " << c;
    std::cout << "\n";

    // ======================================
    // Hit loop
    // ======================================
    for (int h = 0; h < hits; h++)
    {
        std::cout << "number of hits " << (h + 1) << "/" << hits << "\n";

        for (int attackCol : cols)
        {
            std::cout << "column " << attackCol << "\n";

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

            std::cout << "scanning rows:";
            for (int rr : rowsInRange)
                std::cout << " " << rr;
            std::cout << "\n";



        // PROVERA DA LI SE U KOLONI NALAZI TALL PROTIVNICKA KARTA
            bool hasTall = false;
            Tile* tallTile = nullptr;
            for (int r : rowsInRange)
            {
                Tile& t = board.getTile(r, attackCol);
                auto card = t.getFirstEnemyCard(attacker.getOwner());
                if (card && hasModifier(card->getModifiers(), AttackModifier::Tall))
                {
                    hasTall = true;
                    tallTile = &t;
                    break;
                }
            }






            bool hitCard = false;

            for (int r : rowsInRange)
            {
                Tile& tile = board.getTile(r, attackCol);

                if (tile.empty())
                {
                    if (laser && tile.getOwner() != attacker.getOwner())
                    {
                        board.damagePlayer(attacker.getOwner(), attacker.getDamage());
                        std::cout
                            << "laser through empty tile ("
                            << r << "," << attackCol
                            << ") → dmg player for "
                            << attacker.getDamage() << "\n";
                    }
                    continue;
                }

                auto target = tile.getFirstEnemyCard(attacker.getOwner());
                if (!target)
                {
                    std::cout
                        << "tile (" << r << "," << attackCol
                        << ") has no valid target\n";
                    continue;
                }

                // REVERSE MORA DA UDARI TALL KARTU SA TALLTILEA, a ne karte pozadi
                if (reverse && hasTall)
                {
                    target = tallTile->getFirstEnemyCard(attacker.getOwner());
                    if (target)
                    {
                        int hpBefore = target->getHP();
                        target->setHP(hpBefore - attacker.getDamage());
                        std::cout << "Reverse hits Tall only: " << target->getName()
                                  << " HP " << hpBefore << " -> " << target->getHP() << "\n";
                    }
                    break;
                }


                int hpBefore = target->getHP();
                target->setHP(hpBefore - attacker.getDamage());

                std::cout
                    << "HIT " << target->getName()
                    << " at (" << r << "," << attackCol
                    << ") HP " << hpBefore
                    << " -> " << target->getHP() << "\n";

                if (zlatni && target->getHP() < 0)
                {
                    board.damagePlayer(attacker.getOwner(), -target->getHP());

                    std::cout
                        << "zlatni overflow → damage player for: "
                        << -target->getHP() << "\n";
                }


                hitCard = true;
                if (laser)
                {
                    if (hasTall && &tile == tallTile)
                    {
                        std::cout << "Laser hits Tall → stops further tiles\n";
                        break;
                    }
                }
                else
                {
                    std::cout << "stopping (not laser)\n";
                    break;
                }
            }


            if (!hitCard)
            {
                bool enemyInRange = false;

                for (int r : rowsInRange)
                {
                    Tile& t = board.getTile(r, attackCol);
                    if (t.getOwner() != attacker.getOwner())
                    {
                        enemyInRange = true;
                        break;
                    }
                }

                if (enemyInRange)
                {
                    board.damagePlayer(attacker.getOwner(), attacker.getDamage());
                    std::cout << "Player damaged for:" << attacker.getDamage() << std::endl;
                }
            }

        }
        board.cleanupDeadCards();
    }

    std::cout << "\n\n";
}






void AttackSystem::attackJelepeno(Board &board, int row, int col) {
    Tile &origin = board.getTile(row, col);
    auto spellPtr = origin.getAttackTarget();
    if (!spellPtr)
        return;

    Card &spell = *spellPtr;

    std::cout << "[JELEPENO] " << spell.getName()
            << " at (" << row << "," << col << ")\n";

    const std::string &name = spell.getName();

    // ============================
    // IVANA DZARKA – krst
    // ============================
    if (name == "Ivana Dzarka") {
        // centar
        hitTileWithSpell(board, &origin, spell);

        // gore
        hitTileWithSpell(
            board,
            board.getNextActiveTileVertical(row, col, -1),
            spell
        );

        // dole
        hitTileWithSpell(
            board,
            board.getNextActiveTileVertical(row, col, +1),
            spell
        );

        // levo / desno
        hitTileWithSpell(board, board.getTileAtOffset(row, col, 0, -1), spell);
        hitTileWithSpell(board, board.getTileAtOffset(row, col, 0, +1), spell);
    } else {
        std::cout << "⚠ Unknown Jelepeno spell\n";
    }

    // uvek isto
    sf::sleep(sf::seconds(1.5f));
    board.cleanupDeadCards();
    //origin.removeTopCard();

    std::cout << "[JELEPENO] removed\n";
}









void AttackSystem::hitTileWithSpell(Board& board, Tile* tile, Card& spell)
{
    if (!tile)
        return;

    // 1️⃣ uvek prvo tražimo enemy kartu
    auto target = tile->getFirstEnemyCard(spell.getOwner());

    // ako postoji enemy karta → udaramo jede
    if (target)
    {
        int before = target->getHP();
        target->setHP(before - spell.getDamage());

        std::cout << "   hit " << target->getName()
                  << " HP " << before << " -> " << target->getHP() << "\n";
        return;
    }

    // 2️⃣ nema enemy karte → sad gledamo čiji je tile

    // ako je tile protivnikov → dmg player
    if (tile->getOwner() != spell.getOwner())
    {
        board.damagePlayer(spell.getOwner(), spell.getDamage());
        std::cout << "   hit enemy player for " << spell.getDamage() << "\n";
    }
    // ako je moj tile i nema enemy karte → ništa
}








void AttackSystem::attackHemisfera(Board& board, int row, int col)
{
    Tile& origin = board.getTile(row, col);
    auto attackerPtr = origin.getAttackTarget();
    if (!attackerPtr)
        return;

    Card& attacker = *attackerPtr;

    bool rapid  = hasModifier(attacker.getModifiers(), AttackModifier::Rapid);
    bool zlatni = hasModifier(attacker.getModifiers(), AttackModifier::Zlatni);

    int hits = rapid ? attacker.getHitCount() : 1;

    Owner me = attacker.getOwner();

    std::cout << "[HEMISFERA] at (" << row << "," << col << ") hits=" << hits << "\n";

    auto enemyTiles = board.getActiveEnemyTiles(me);

    for (int h = 0; h < hits; h++)
    {
        for (const Tile* tile : enemyTiles)
        {
            // traži prvu enemy kartu (ignoriše parasite)
            auto target = tile->getFirstEnemyCard(me);

            if (target)
            {
                int before = target->getHP();
                target->setHP(before - attacker.getDamage());

                /*std::cout << "  hit " << target->getName()
                          << " at (" << tile->getRow() << "," << tile->getCol()
                          << ") HP " << before << " -> " << target->getHP() << "\n";*/

                if (zlatni && target->getHP() < 0)
                {
                    board.damagePlayer(me, -target->getHP());
                    target->setHP(0);
                }
            }
            else
            {
                board.damagePlayer(me, attacker.getDamage());

                std::cout << "  hit enemy player from tile\n";
            }
        }
        board.cleanupDeadCards();
    }
}








std::vector<AttackSystem::SniperTarget>
AttackSystem::collectSniperTargets(Board& board, Owner me, int row, int col)
{
    std::vector<SniperTarget> targets;

    for (int r = 0; r < Board::ROWS; r++)
    {
        if (!board.getTile(r, 0).isActive())
            continue;

        for (int c = 0; c < Board::COLS; c++)
        {
            Tile& tile = board.getTile(r, c);
            auto target = tile.getFirstEnemyCard(me);
            if (!target)
                continue;

            int dist = std::abs(r - row) + std::abs(c - col);
            targets.push_back({ &tile, target, dist });
        }
    }

    return targets;
}








void AttackSystem::dealDamageToCard(Board& board, Card& sniper, std::shared_ptr<Card> target)
{
    Owner me = sniper.getOwner();

    bool zlatni = hasModifier(sniper.getModifiers(), AttackModifier::Zlatni);

    int before = target->getHP();
    target->setHP(before - sniper.getDamage());

    std::cout << "   hit " << target->getName()
              << " HP " << before
              << " -> " << target->getHP() << "\n";

    if (zlatni && target->getHP() < 0)
    {
        board.damagePlayer(me, -target->getHP());
        std::cout << "   zlatni overflow → dmg player for "
                  << -target->getHP() << "\n";
        target->setHP(0);
    }
}







void AttackSystem::attackSniper(Board& board, int row, int col)
{
    Tile& origin = board.getTile(row, col);
    auto attackerPtr = origin.getAttackTarget();
    if (!attackerPtr)
        return;

    Card& sniper = *attackerPtr;
    Owner me = sniper.getOwner();

    bool rapid = hasModifier(sniper.getModifiers(), AttackModifier::Rapid);
    int hits = rapid ? sniper.getHitCount() : 1;

    const std::string& name = sniper.getName();

    std::cout << "[SNIPER] " << name
              << " at (" << row << "," << col << ") hits=" << hits << "\n";


    // =========================
    // NIKOLICA – najbliža MUŠKA
    // =========================
    if (name == "Nikolica Prikolica")
    {
        for (int i = 0; i < hits; i++)
        {
            auto targets = collectSniperTargets(board, me, row, col);

            if (targets.empty())
            {
                std::cout << "   no valid targets\n";
                return;
            }

            std::vector<SniperTarget> maleTargets;
            for (auto& t : targets)
                if (t.card->getPol())
                    maleTargets.push_back(t);

            if (maleTargets.empty())
            {
                std::cout << "   no male targets\n";
                return;
            }

            auto best = std::min_element(
                maleTargets.begin(),
                maleTargets.end(),
                [](const SniperTarget& a, const SniperTarget& b) {
                    return a.dist < b.dist;
                }
            );

            dealDamageToCard(board, sniper, best->card);
            board.cleanupDeadCards();
        }
        board.cleanupDeadCards();
        return;
    }

    // =========================
    // PHESIC – svi MUŠKI
    // =========================
    if (name == "Phesic Nina")
    {
        for (int i = 0; i < hits; i++)
        {
            auto targets = collectSniperTargets(board, me, row, col);

            if (targets.empty())
            {
                std::cout << "   no valid targets\n";
                return;
            }

            std::vector<SniperTarget> maleTargets;
            for (auto& t : targets)
                if (t.card->getPol())
                    maleTargets.push_back(t);

            if (maleTargets.empty())
            {
                std::cout << "   no male targets\n";
                return;
            }

            for (auto& t : maleTargets)
                dealDamageToCard(board, sniper, t.card);
            board.cleanupDeadCards();
        }

        board.cleanupDeadCards();
        return;
    }

    std::cout << "⚠ Unknown Sniper\n";
}










void AttackSystem::attackPump(Board& board, int row, int col)
{
    Tile& origin = board.getTile(row, col);
    auto pumpPtr = origin.getAttackTarget();
    if (!pumpPtr)
        return;

    Card& pump = *pumpPtr;
    Owner me = pump.getOwner();

    Player& myPlayer =
        (me == Owner::Player1 ? board.getPlayer1() : board.getPlayer2());

    Player& enemyPlayer =
        (me == Owner::Player1 ? board.getPlayer2() : board.getPlayer1());

    const std::string& name = pump.getName();

    std::cout << "[PUMP] " << name
              << " at (" << row << "," << col << ")\n";

    // =====================================
    // KONSTANTIN DAJAMOND → +1
    // =====================================
    if (name == "Konstantin Dajamond")
    {
        myPlayer.addElixir(1);
        std::cout << "   +1 elixir\n";
        return;
    }

    // =====================================
    // NIKOCADO AVOCADO → +1 (posebna logika kasnije)
    // =====================================
    if (name == "Nikocado Avocado")
    {
        myPlayer.addElixir(1);
        std::cout << "   +1 elixir (Nikocado)\n";
        return;
    }

    // =====================================
    // SLANINA → ukradi 1
    // =====================================
    if (name == "Slanina")
    {
        enemyPlayer.setElixir(enemyPlayer.getElixir() - 1); // moze u minus
        myPlayer.addElixir(1);

        std::cout << "   stole 1 elixir from enemy, current elixir:" << myPlayer.getElixir() << std::endl;
        return;
    }

    // =====================================
    // CHAPIKA → zavisi od age
    // =====================================
    if (name == "Chapika")
    {
        int age = pump.getAge();
        int gain = 0;

        if (age == 0) gain = 1;
        else if (age == 1) gain = 2;
        else if (age == 2) gain = 3;
        else if (age == 4) gain = 5;

        if (gain > 0)
        {
            myPlayer.addElixir(gain);
            std::cout << "   age=" << age << " → +" << gain << " elixir\n";
        }
        else
        {
            std::cout << "   age=" << age << " → no elixir\n";
        }

        return;
    }


    // =====================================
    // KRISTINA RINGLO → conditional pump
    // =====================================
    if (name == "Kristina Ringlo")
    {
        int maxHp = pump.getMaxHp();
        int hp = pump.getHP();

        if (hp < maxHp)
        {
            myPlayer.addElixir(2);
            pump.setHP(hp - 1);

            std::cout << "   damaged → +2 elixir, self -1 HP\n";
        }
        else
        {
            myPlayer.addElixir(1);
            std::cout << "   full HP → +1 elixir\n";
        }
        return;
    }


    std::cout << "⚠ Unknown Pump card\n";
}