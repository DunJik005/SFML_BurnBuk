//
// Created by Lazar Bisenic on 18. 1. 2026..
//

#ifndef SFMLPROJECT_TILE_H
#define SFMLPROJECT_TILE_H
#include <list>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Card.h"
#include "ReturnButton.h"


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
    sf::Font& getFont();


    bool isActive() const;

    // ---------- Card stack ----------
    bool canPlace(const std::shared_ptr<Card>& card) const;
    bool placeCard(const std::shared_ptr<Card>& card);
    bool canReturnCard(Owner currentPlayer) const;
    int getPlacedThisTurnCount() const { return placedThisTurnCount; }
    void setPlacedThisTurnCount(int n) {
        placedThisTurnCount = n;
    }

    const std::list<std::shared_ptr<Card>>& getAllCards() const;


    std::shared_ptr<Card> getFirstEnemyCard(Owner owner) const;


    std::shared_ptr<Card> removeTopCard();
    std::shared_ptr<Card> returnTopCardToHand();
    std::shared_ptr<Card> topCard() const;
    std::shared_ptr<Card> bottomCard() const;

    bool empty() const;
    size_t cardCount() const;

    // ---------- Combat helpers ----------
    bool hasLeech() const;
    void cleanupDeadCards();
    void updateStatsValue();

    // ---------- Visual ----------

    void setTexture(sf::Texture* texture);
    void setHovered(bool h);
    bool isHovered() const { return hovered; }

    void setPosition(float x, float y);
    void setSize(float w, float h);
    void setFont(sf::Font& f);

    sf::FloatRect getBounds() const;

    void draw(sf::RenderWindow& window);
    std::shared_ptr<Card> getAttackTarget();

    bool handleReturnClick(float x, float y) const;

private:
    // ---------- Core data ----------
    State state = State::Inactive;
    Owner owner = Owner::None;

    // bottom -> front, top -> back
    std::list<std::shared_ptr<Card>> cards;

    int placedThisTurnCount = 0;


    // ---------- Visual ----------
    sf::RectangleShape shape;
    sf::RectangleShape statsBar;
    sf::Text statsText;
    sf::Font* font = nullptr;

    bool hovered = false;

    // ---------- Internal helpers ----------
    void fitCardToTile(const std::shared_ptr<Card>& card);

    ReturnButton returnButton;

};


#endif //SFMLPROJECT_TILE_H