//
// Created by Teodora Mladenovic on 3. 1. 2026..
//
#include "CardRender.h"
#include <iostream>
#include "NeutralTexture.h"

// ---------- shared resources ----------

sf::Font& CardRenderer::getFont() {
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

sf::Texture& CardRenderer::getStarTexture() {
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
// ---------- ctor ----------

CardRenderer::CardRenderer()
    : artSprite(getNeutralTexture()),
      frameSprite(getNeutralTexture()),
      starSprite(getStarTexture()),
      statsText(getFont()),
      nameText(getFont()),
      descriptionText(getFont()),
      descriptionBox(getFont())
{
    statsText.setFillColor(sf::Color::Black);
    descriptionText.setFillColor(sf::Color::Black);
    descriptionText.setLineSpacing(1.2f);
    nameText.setFillColor(sf::Color::Black);
    starSprite.setScale({starsScaleFactor, starsScaleFactor});
    descriptionBox.setCharacterSize(descCharSize);
    descriptionBox.setVisibleLines(5);
}

// ---------- api ----------
void CardRenderer::scrollDescription(float delta) {
    descriptionBox.scroll(delta * scrollSpeed);
}
void CardRenderer::update(float dt) {
    descriptionBox.update(dt);
}

void CardRenderer::setCard(const Card& c) {
    card = &c;

    artSprite.setTexture(*card->getTexture(), true);
    frameSprite.setTexture(Card::getRarityFrame(card->getRarity()), true);

    statsText.setCharacterSize(100);
    statsText.setString(
        "HP: " + std::to_string(card->getHP()) +
        "   DMG: " + std::to_string(card->getDamage())
    );
    float padding = 100.f;
    float cardWidth = frameSprite.getTexture().getSize().x;

    float textBoxWidth = cardWidth - padding * 2.f;

    descriptionBox.setBoxWidth(textBoxWidth);
    descriptionBox.setText(card->getDescription());

    nameText.setCharacterSize(128);
    nameText.setString(card->getName());
}

void CardRenderer::setScale(float s) {
    scale = s;
}

void CardRenderer::setPosition(sf::Vector2f pos) {
    position = pos;

    // --- stats text bottom-right ---
    sf::Vector2f cardSize(
        static_cast<float>(frameSprite.getTexture().getSize().x),
        static_cast<float>(frameSprite.getTexture().getSize().y)
    );

    sf::FloatRect statsBounds = statsText.getLocalBounds();

    float padding = 120.f;

    sf::Vector2f statsLocalPos(
     cardSize.x - statsBounds.size.x  - padding - statsBounds.position.x ,
     cardSize.y - statsBounds.size.y - padding - statsBounds.position.y
 );

    statsText.setPosition(statsLocalPos);

    nameText.setPosition({padding,  padding / 2.f});

    descriptionBox.setPosition({
    100.f,
    frameSprite.getTexture().getSize().y - 520.f
});

}

// ---------- draw ----------

void CardRenderer::draw(sf::RenderTarget& target) const {
    if (!card) return;

    sf::RenderStates states;
    states.transform.translate(position);
    states.transform.scale({scale, scale});

    target.draw(artSprite, states);
    target.draw(frameSprite, states);
    target.draw(statsText, states);
    target.draw(nameText, states);
    descriptionBox.draw(target, states);


    int cost = card->getCost();
    float cardW = frameSprite.getTexture().getSize().x;
    float padding = 240.f;

    if (cost <= 10) {
        float starSpacing = 110.f; // razmak izmedju zvezdica
        // float starOffsetY = 0.f;
        for (int i = 0; i < cost; i++) {
            sf::RenderStates s = states;
            s.transform.translate({
                cardW - padding - i * starSpacing ,
                padding
            });
            target.draw(starSprite, s);
        }
    } else {
        float starOffsetX = cardW - padding; // horizontalni offset za zvezdicu
        float starOffsetY = padding;         // vertikalni offset

        // crta se jedna zvezdica
        sf::RenderStates s = states;
        s.transform.translate({ starOffsetX, starOffsetY });
        target.draw(starSprite, s);

        sf::Text costText(getFont());
        costText.setCharacterSize(140);
        costText.setFillColor(sf::Color::Black);
        costText.setString(std::to_string(cost) + "x");

        sf::FloatRect textBounds = costText.getLocalBounds();
        sf::FloatRect starBounds = starSprite.getLocalBounds();
        // automatski pomeramo ulevo prema širini teksta
        float x = -textBounds.size.x - 20.f; // 20.f je razmak od zvezdice

        // Y: poravnamo sa vrhom zvezdice
        float y = -50.f; // u lokalnom prostoru zvezdice, 0 je vrh
        costText.setPosition({x, y});

        target.draw(costText, s);
    }
}

