#ifndef SFMLPROJECT_HAND_H
#define SFMLPROJECT_HAND_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "CardHand.h"
#include "OnResize.h"

class Hand : OnResize{
public:
    Hand() = default;

    // init
    // layout
    void recalcLayout(float windowWidth);

    void onResize(float windowWidth, float windowHeight) override;


    // input
    std::shared_ptr<CardHand> handleClick(float x, float y);

    // modify
    void addCard(const std::shared_ptr<CardHand>& card, float windowWidth, float windowHeight);
    void removeHand(const std::shared_ptr<CardHand>& card, float windowWidth);

    void setVisibleOwner(Owner o) { visibleOwner = o; }

    // render
    void draw(sf::RenderWindow& window,
              const std::shared_ptr<CardHand>& selected = nullptr);

/* KOD ZA DRUGI HAND GORE NAOPAK
    void setActive(bool value) { isActive = value; }
*/

private:

    Owner visibleOwner = Owner::Player1;
/*
    // dodato za dupli hand i obrnut
    bool isActive = true;
    bool drawOnTop = false;
*/
    std::vector<std::shared_ptr<CardHand>> cards;

    float baseY = 0.f;
    float spacing = 50.f;
};

#endif