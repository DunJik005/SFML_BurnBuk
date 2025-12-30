#include "CardView.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include "NeutralTexture.h"
// ---------------- shared resources ----------------

sf::Font& CardView::getFont() {
    static sf::Font font;
    static bool initialized = false;

    if (!initialized) {
        if (!font.openFromFile("assets/fonts/MatrixSmallCaps.ttf")) {
            std::cerr << "greska: font nije ucitan\n";
        }
        initialized = true;
    }
    return font;
}

sf::Texture& CardView::getStarTexture() {
    static sf::Texture tex;
    static bool initialized = false;

    if (!initialized) {
        if (!tex.loadFromFile("assets/frames/star.png")) {
            std::cerr << "greska: star.png nije ucitan\n";
        }
        initialized = true;
    }
    return tex;
}

// ---------------- ctor ----------------
CardView::CardView()
    : artSprite(getNeutralTexture()),   // dummy texture, neće se crtati dok nema karte
      frameSprite(getNeutralTexture()),
      starSprite(getStarTexture()),
      statsText(getFont()),
      descriptionText(getFont())
{
    statsText.setFillColor(sf::Color::Black);
    descriptionText.setFillColor(sf::Color::Black);

    descriptionText.setLineSpacing(1.2f);

    starSprite.setScale({starsScaleFactor, starsScaleFactor});
}

// ---------------- api ----------------

void CardView::show(const Card& cardObj) {
    card = &cardObj;
    visible = true;


    // ART
    // artSprite.setTexture(card->getArtTexture());

    artSprite.setTexture(*card->getTexture());
    artSprite.setOrigin({0.f, 0.f});
    artSprite.setTextureRect(
        sf::IntRect({0, 0},
        sf::Vector2i(card->getTexture()->getSize()))
    );
    // FRAME
    frameSprite.setTexture(Card::getRarityFrame(card->getRarity()));
    frameSprite.setOrigin({0.f, 0.f});
    frameSprite.setTextureRect(
        sf::IntRect({0, 0},
        sf::Vector2i(card->getTexture()->getSize()))
    );


    // TEXT
    statsText.setCharacterSize(36);
    statsText.setString(
        "HP: " + std::to_string(card->getHP()) +
        "   DMG: " + std::to_string(card->getDamage())
    );

    descriptionText.setCharacterSize(28);
    descriptionText.setString(card->getDescription());

    if (lastWinW > 0 && lastWinH > 0)
        onResize(lastWinW, lastWinH);
}

void CardView::hide() {
    visible = false;
    card = nullptr;
}

// ---------------- resize / layout ----------------

void CardView::onResize(float winW, float winH) {
    lastWinW = winW;
    lastWinH = winH;
    if (!visible || !card) return;

    float panelW = winW * 0.33f - margin * 2.f;
    float panelH = winH - margin * 2.f;

    auto texSize = frameSprite.getTexture().getSize();
    float cardW = static_cast<float>(texSize.x);
    float cardH = static_cast<float>(texSize.y);

    scaleFactor = std::min(panelW / cardW, panelH / cardH);

    panelPos.x = winW * 0.67f + margin;
    panelPos.y = (winH - cardH * scaleFactor) * 0.5f;

    statsText.setPosition({
        panelPos.x + 20.f,
        panelPos.y + 20.f
    });

    descriptionText.setPosition({
        panelPos.x + 20.f,
        panelPos.y + cardH * scaleFactor - 140.f
    });
}
// ---------------- draw ----------------

void CardView::draw(sf::RenderWindow& window) const {
    if (!visible || !card) return;

    sf::RenderStates states;
    states.transform.translate(panelPos);
    states.transform.scale({scaleFactor, scaleFactor});

    // art
    window.draw(artSprite, states);

    // frame
    window.draw(frameSprite, states);

    // cost (stars)
    int cost = card->getCost();
    auto texSize = frameSprite.getTexture().getSize();
    float cardW = static_cast<float>(texSize.x);
    float padding = 140.f;

    if (cost <= 10) {
        for (int i = 0; i < cost; i++) {
            sf::RenderStates s = states;
            s.transform.translate({
                cardW - padding - i * 110.f,
                padding + 100.f
            });
            window.draw(starSprite, s);
        }
    } else {
        sf::Text costText(getFont());
        costText.setCharacterSize(static_cast<unsigned>(120 * scaleFactor));
        costText.setFillColor(sf::Color::Black);
        costText.setString(std::to_string(cost) + "x");

        sf::RenderStates s = states;
        s.transform.translate({
            cardW - padding,
            padding + 100.f
        });

        window.draw(starSprite, s);

        costText.setPosition({
            panelPos.x + (cardW - padding - 220.f) * scaleFactor,
            panelPos.y + (padding + 70.f) * scaleFactor
        });

        window.draw(costText);
    }
    // text
    window.draw(statsText);
    window.draw(descriptionText);
}