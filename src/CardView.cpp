// #include "CardView.h"
// #include <iostream>
// #include <SFML/Graphics.hpp>
//
// CardView::CardView(const CardData& data)
//     : data(data), baseSprite(), artSprite(), nameText(), statText()
// {
//     // --- FONT ---
//     if (!font.loadFromFile("assets/fonts/arial.ttf"))
//         std::cerr << "Font error\n";
//
//     // --- BASE TEXTURE --- (rarity frame)
//     std::string basePath;
//     switch (data.getRarity()) {
//         case Rarity::Common:    basePath = "assets/cards/common_frame.png"; break;
//         case Rarity::Rare:      basePath = "assets/cards/rare_frame.png"; break;
//         case Rarity::Epic:      basePath = "assets/cards/epic_frame.png"; break;
//         case Rarity::Legendary: basePath = "assets/cards/legendary_frame.png"; break;
//         case Rarity::Hero:      basePath = "assets/cards/hero_frame.png"; break;
//     }
//
//     if (!baseTexture.loadFromFile(basePath))
//         std::cerr << "Base texture error: " << basePath << "\n";
//     baseSprite.setTexture(baseTexture);
//     baseSprite.setScale(0.6f, 0.6f);
//
//     // --- ART ---
//     if (!artTexture.loadFromFile(data.artTexturePath))
//         std::cerr << "Art texture error: " << data.artTexturePath << "\n";
//     artSprite.setTexture(artTexture);
//     artSprite.setScale(0.5f, 0.5f);
//
//     // --- NAME ---
//     nameText.setFont(font);
//     nameText.setString(data.getName());
//     nameText.setCharacterSize(22);
//
//     // --- STATS ---
//     statText.setFont(font);
//     statText.setCharacterSize(20);
//     statText.setString(
//         "HP: " + std::to_string(data.getHealth()) +
//         "  DMG: " + std::to_string(data.getDamage())
//     );
//
//     // --- STARS ---
//     if (!starTexture.loadFromFile("assets/cards/star.png"))
//         std::cerr << "Star texture missing\n";
//
//     for (int i = 0; i < data.getCost(); ++i) {
//         sf::Sprite star;
//         star.setTexture(starTexture);
//         star.setScale(0.1f, 0.1f);
//         star.setPosition(120.f + i * 20.f, 120.f); // horizontalni razmak
//         stars.push_back(star);
//     }
// }
//
// void CardView::draw(sf::RenderWindow& window)
// {
//     window.draw(baseSprite);
//     window.draw(artSprite);
//     for (auto& star : stars)
//         window.draw(star);
//     window.draw(nameText);
//     window.draw(statText);
// }
