#ifndef CARDRENDERER_H
#define CARDRENDERER_H

#include <SFML/Graphics.hpp>
#include "Card.h"
#include "Textbox.h"
enum class RenderMode {
    Front,
    Back
};
class CardRenderer {
public:
    // ---------- Ctor ----------
    CardRenderer();
    // ---------- Settes ----------
    void setCard(const Card& c);
    void setScale(float s);
    void setPosition(sf::Vector2f pos);
    void setRenderMode(RenderMode m) { mode = m; }
    // ---------- Draw ----------
    void draw(sf::RenderTarget& target) const;

    // ---------- Getters ----------
    sf::FloatRect getBounds() const { return frameSprite.getGlobalBounds(); } //uključuje poziciju i transformacije

    // ---------- Description Scroll ----------
    void scrollDescription(float delta);
    void update(float dt) ;

private:
    // ---------- External parameters ----------
    const Card* card = nullptr;
    RenderMode mode = RenderMode::Front;
    Textbox descriptionBox;

    // ---------- Sprites ----------
    sf::Sprite artSprite;
    sf::Sprite frameSprite;
    mutable sf::Sprite starSprite;
    sf::Sprite backSprite;

    // ---------- Texts ----------
    sf::Text statsText;
    sf::Text descriptionText;
    sf::Text nameText;

    // ---------- Description parameters ----------
    float descCharSize = 90.f;
    int visibleLines = 4;
    float scrollSpeed = 20.f;

    // ---------- Scaling and positioning ----------
    float scale = 1.f;
    sf::Vector2f position{0.f, 0.f};
    float starsScaleFactor = 0.22f;

    // ---------- Shared Resources ----------
    static sf::Font& getFont();
    static sf::Font& getDescFont();
    static sf::Texture& getStarTexture();
    static sf::Texture& getCardBackTexture();
};

#endif