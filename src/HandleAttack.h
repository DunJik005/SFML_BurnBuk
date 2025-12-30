//
// Created by lazab on 12/20/2025.
//

#ifndef SFMLPROJECT_HANDLEATTACK_H
#define SFMLPROJECT_HANDLEATTACK_H
#include "Board.h"
#include "AttackSystem.h"
#include <vector>


class HandleAttack {

public:
    HandleAttack(Board& b): board(b){}

    void executeAttacks();

private:
    Board& board;

    static const std::vector<std::pair<int,int>> attackPairs;

};


#endif //SFMLPROJECT_HANDLEATTACK_H