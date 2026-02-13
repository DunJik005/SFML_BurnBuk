#ifndef SFMLPROJECT_HAND_H
#define SFMLPROJECT_HAND_H

#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>
#include "CardHand.h"
#include "OnResize.h"

class Hand : OnResize{
public:
    // ---------- Ctor ----------
    Hand() = default;
    // ---------------- LAYOUT ----------------
    void onResize(float windowWidth, float windowHeight) override;

    // ---------- Handle Click ----------
    std::shared_ptr<CardHand> handleClick(float x, float y);

    // ---------- Modify ----------
    void addCard(const std::shared_ptr<CardHand>& card, float windowWidth, float windowHeight);
    void removeHand(const std::shared_ptr<CardHand>& card);
    void setVisibleOwner(Owner o) { visibleOwner = o; }

    // ---------- Draw ----------
    void draw(sf::RenderWindow& window, const std::shared_ptr<CardHand>& selected = nullptr);

    // ---------- Active Hand ----------
    void setActive(bool value);
    bool getActive(){ return isActive; }


private:
    // ---------- Owner ----------
    Owner visibleOwner = Owner::Player1;
    bool isActive = true;
    // ---------- All cards ----------
    std::vector<std::shared_ptr<CardHand>> cards;

    // ---------- Scaling and positioning parameters ----------
    float cardScale = 0.2f;
    float overlapRatio = 0.25f; // koliko se preklapaju
    float centerX = 0.f;
    float zoneHeight = 0.f;
    float baseY = 0.f; //odakle krece pozicija karte

    // ---------- Last Window Size ----------
    float lastWindowWidth = 0.f;
    float lastWindowHeight = 0.f;

};

#endif