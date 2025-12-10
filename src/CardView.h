// #pragma once
// #include <SFML/Graphics.hpp>
// #include "CardData.h"
// #include <vector>
// #include <string>
//
// class CardView {
// private:
//     const CardData& data;             // reference na podatke karte
//
//     // sve teksture kao članovi da žive dovoljno dugo
//     sf::Texture baseTexture;
//     sf::Texture artTexture;
//     sf::Texture starTexture;
//
//     sf::Sprite baseSprite;            // frame (rarity)
//     sf::Sprite artSprite;             // art u sredini
//     std::vector<sf::Sprite> stars;    // zvezdice
//
//     sf::Font font;
//     sf::Text nameText;
//     sf::Text statText;
//
//     sf::Vector2f position;
//
//     // veličina karte (u pikselima) - treba da odgovara tvojim tile dimenzijama
//     static constexpr float cardWidth  = 200.f;
//     static constexpr float cardHeight = 250.f;
//
// public:
//     explicit CardView(const CardData& data);
//
//     void setPosition(const sf::Vector2f& pos);
//     const sf::Vector2f& getPosition() const { return position; }
//
//     void draw(sf::RenderWindow& window);
// };
