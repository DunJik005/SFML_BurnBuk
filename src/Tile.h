//
// Created by Lazar Bisenic on 18. 1. 2026..
//

#ifndef SFMLPROJECT_TILE_H
#define SFMLPROJECT_TILE_H
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Card.h"


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

    sf::Vector2f getPosition() const;
    sf::Vector2f getSize() const;


    bool isActive() const;

    // ---------- Card stack ----------
    bool canPlace(const std::shared_ptr<Card>& card) const;
    bool placeCard(const std::shared_ptr<Card>& card);

    std::shared_ptr<Card> removeTopCard();
    std::shared_ptr<Card> topCard() const;
    std::shared_ptr<Card> bottomCard() const;

    bool empty() const;
    size_t cardCount() const;

    // ---------- Combat helpers ----------
    bool hasLeech() const;
    void cleanupDeadCards();

    // ---------- Visual ----------

    void setTexture(sf::Texture* texture);
    void setHovered(bool h);

    void setPosition(float x, float y);
    void setSize(float w, float h);
    sf::FloatRect getBounds() const;

    void draw(sf::RenderWindow& window);
    std::shared_ptr<Card> getAttackTarget();


private:
    // ---------- Core data ----------
    State state = State::Inactive;
    Owner owner = Owner::None;

    // bottom -> front, top -> back
    std::list<std::shared_ptr<Card>> cards;

    // ---------- Visual ----------
    sf::RectangleShape shape;

    bool hovered = false;



private:
    // ---------- Internal helpers ----------
    void fitCardToTile(const std::shared_ptr<Card>& card);
};


#endif //SFMLPROJECT_TILE_H