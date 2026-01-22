//
// Created by Teodora Mladenovic on 19. 1. 2026..
//

#ifndef SFMLPROJECT_TEXTBOX_H
#define SFMLPROJECT_TEXTBOX_H
#include <SFML/Graphics.hpp>

class Textbox {
public:
    Textbox(const sf::Font& font);

    void setCharacterSize(unsigned int size);
    void setBoxWidth(float w);
    void setVisibleLines(int lines);
    void setPosition(sf::Vector2f pos);

    void setText(const std::string& text);

    void scroll(float delta);
    void update(float dt);
    void draw(sf::RenderTarget& target, const sf::RenderStates& states) const;

private:
    std::string wrapText(const std::string& str) const;

    const sf::Font& font;
    mutable sf::Text lineText;

    std::vector<std::string> lines;

    float boxWidth = 0.f;
    float lineHeight = 0.f;

    int visibleLines = 5;

    float scrollPos = 0.f;
    float targetScroll = 0.f;
    float scrollSpeed = 20.f;

    sf::Vector2f position;
};



#endif //SFMLPROJECT_TEXTBOX_H