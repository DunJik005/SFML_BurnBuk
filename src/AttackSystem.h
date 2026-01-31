#ifndef SFMLPROJECT_ATTACKSYSTEM_H
#define SFMLPROJECT_ATTACKSYSTEM_H

#include "Board.h"

class AttackSystem {
public:
    static void resolveAttack(Board& board, int row, int col);

private:
    static void attackChomper(Board& board, int index);
    static void attackPeashooter(Board& board, int index);
    static void attackNut(Board& board, int index);
    static void attackJelepeno(Board& board, int index);
    //static void linearAttack(Board& board, Card& attacker, int index);
    static void linearAttack(Board& board, int row, int col);





    //smer napada
    static int getDirection(const Card& card);
    static int getRange(BaseAttack base);
    static bool hasModifier(AttackModifier mods, AttackModifier flag);

};


#endif //SFMLPROJECT_ATTACKSYSTEM_H