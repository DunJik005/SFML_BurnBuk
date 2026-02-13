#ifndef SFMLPROJECT_ATTACKSYSTEM_H
#define SFMLPROJECT_ATTACKSYSTEM_H

class Board;   // 👈 KLJUČNO
class Tile;
class Card;
#include "Card.h"


class AttackSystem {
public:
    static void resolveAttack(Board& board, int row, int col);


    static void attackJelepeno(Board& board, int row, int col);
    //static void linearAttack(Board& board, Card& attacker, int index);
    static void linearAttack(Board& board, int row, int col);
    static void attackHemisfera(Board& board, int row, int col);
    static void attackSniper(Board& board, int row, int col);
    static void attackPump(Board& board, int row, int col);





    struct SniperTarget {
        Tile* tile;
        std::shared_ptr<Card> card;
        int dist;
    };

    static std::vector<SniperTarget> collectSniperTargets(Board& board, Owner me, int row, int col);

    static void dealDamageToCard(Board& board, Card& sniper, std::shared_ptr<Card> target);




private:
    static void hitTileWithSpell(Board& board, Tile* tile, Card& spell);
    //smer napada
    static int getDirection(const Card& card);
    static int getRange(BaseAttack base);
    static bool hasModifier(AttackModifier mods, AttackModifier flag);

};


#endif //SFMLPROJECT_ATTACKSYSTEM_H