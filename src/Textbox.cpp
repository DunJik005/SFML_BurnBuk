#include "Textbox.h"
#include <sstream>
#include <algorithm>
#include <cmath>

Textbox::Textbox(const sf::Font& f)
    : font(f), lineText(font) {
    lineText.setFillColor(sf::Color::Black);
}

void Textbox::setCharacterSize(unsigned int size) {
    lineText.setCharacterSize(size);
    lineHeight = font.getLineSpacing(size) * lineSpacingFactor;
}

void Textbox::setBoxWidth(float w) {
    boxWidth = w;
}

void Textbox::setVisibleLines(int linesCount) {
    visibleLines = linesCount;
}

void Textbox::setPosition(sf::Vector2f pos) {
    position = pos;
}

void Textbox::setText(const std::string& text) {
    lines.clear();

    std::string wrapped = wrapText(text);
    std::stringstream ss(wrapped);
    std::string line;

    while (std::getline(ss, line)) {
        lines.push_back(line);
    }

    scrollPos = 0.f;
    targetScroll = 0.f;
}

std::string Textbox::wrapText(const std::string& str) const {
    sf::Text tmp(font, "", lineText.getCharacterSize());

    std::string result, word, line;

    for (char c : str) {
        if (c == ' ' || c == '\n') {
            tmp.setString(line + word);
            if (tmp.getLocalBounds().size.x > boxWidth) {
                result += line + "\n";
                line = word + " ";
            } else {
                line += word + " ";
            }
            word.clear();

            if (c == '\n') {
                result += line + "\n";
                line.clear();
            }
        } else {
            word += c;
        }
    }

    tmp.setString(line + word);
    if (tmp.getLocalBounds().size.x > boxWidth) {
        result += line + "\n" + word;
    } else {
        result += line + word;
    }

    return result;
}
void Textbox::setLineSpacing(float factor) {
    lineSpacingFactor = factor;
    lineHeight = font.getLineSpacing(lineText.getCharacterSize()) * lineSpacingFactor;
}

void Textbox::scroll(float delta) {
    targetScroll += delta;
    float maxScroll = std::max(0.f, (float)lines.size() - visibleLines);
    targetScroll = std::clamp(targetScroll, 0.f, maxScroll);
}

void Textbox::update(float dt) {
    float diff = targetScroll - scrollPos;
    scrollPos += diff * scrollSpeed * dt;

    if (std::abs(diff) < 0.001f)
        scrollPos = targetScroll;
}

void Textbox::draw(sf::RenderTarget& target, const sf::RenderStates& states) const {
    int firstLine = (int)scrollPos;
    float offset = scrollPos - firstLine;

    for (int i = 0; i < visibleLines ; ++i) {
        int idx = firstLine + i;
        if (idx >= lines.size()) break;

        lineText.setString(lines[idx]);

        sf::RenderStates s = states;
        s.transform.translate({
            position.x,
            position.y + (i - offset) * lineHeight
        });

        target.draw(lineText, s);
    }
}
