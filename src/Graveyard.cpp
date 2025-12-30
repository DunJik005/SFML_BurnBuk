#include "Graveyard.h"

void Graveyard::addCard(const std::shared_ptr<Card>& card) {
    cards.push_back(card);
}

int Graveyard::getCount() const {
    return (int) cards.size();
}