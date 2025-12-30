#ifndef SFMLPROJECT_GRAVEYARD_H
#define SFMLPROJECT_GRAVEYARD_H

#include <vector>
#include <memory>
#include "Card.h"

class Graveyard {
public:
    void addCard(const std::shared_ptr<Card>& card);
    int getCount() const;

private:
    std::vector<std::shared_ptr<Card>> cards;
};

#endif
