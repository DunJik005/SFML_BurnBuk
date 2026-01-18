//
// Created by Lazar Bisenic on 18. 1. 2026..
//

#ifndef SFMLPROJECT_TILE_H
#define SFMLPROJECT_TILE_H
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>
#include "CardBoard.h"


class Tile {

public:
    enum class State {
        Inactive,
        Active
    };

public:

    Tile();

    // ---------- State / Ownership ----------
    void setState(State s);
    State getState() const;

    void setOwner(Owner o);
    Owner getOwner() const;

    bool isActive() const;

    // ---------- Card stack ----------
    bool canPlace(const std::shared_ptr<CardBoard>& card) const;
    bool placeCard(const std::shared_ptr<CardBoard>& card);

    std::shared_ptr<CardBoard> removeTopCard();
    std::shared_ptr<CardBoard> topCard() const;
    std::shared_ptr<CardBoard> bottomCard() const;

    bool empty() const;
    size_t cardCount() const;

    // ---------- Combat helpers ----------
    bool hasLeech() const;
    void cleanupDeadCards();

    // ---------- Visual ----------
    void setPosition(float x, float y);
    void setSize(float w, float h);
    sf::FloatRect getBounds() const;

    void draw(sf::RenderWindow& window);
    std::shared_ptr<CardBoard> getAttackTarget();


private:
    // ---------- Core data ----------
    State state = State::Inactive;
    Owner owner = Owner::None;

    // bottom -> front, top -> back
    std::list<std::shared_ptr<CardBoard>> cards;

    // ---------- Visual ----------
    sf::RectangleShape shape;

private:
    // ---------- Internal helpers ----------
    void fitCardToTile(const std::shared_ptr<CardBoard>& card);
};


#endif //SFMLPROJECT_TILE_H