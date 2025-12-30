//
// Created by lazab on 12/22/2025.
//

#ifndef SFMLPROJECT_ATTACKSYSTEM_H
#define SFMLPROJECT_ATTACKSYSTEM_H

#include "Board.h"

class AttackSystem {
public:
    static void resolveAttack(Board& board, int attackerIndex);

private:
    static void attackChomper(Board& board, int index);
    static void attackPeashooter(Board& board, int index);
    static void attackNut(Board& board, int index);
    static void attackJelepeno(Board& board, int index);




    //smer napada
    static int getDirection(const Card& card);


};


#endif //SFMLPROJECT_ATTACKSYSTEM_H