#ifndef BURNBOOKALFA_RETURNBUTTON_H
#define BURNBOOKALFA_RETURNBUTTON_H
#include <SFML/Graphics.hpp>

class ReturnButton {
public:
    ReturnButton();

    void setBounds(const sf::FloatRect& cardBounds);
    void setVisible(bool v);

    void draw(sf::RenderWindow& window);
    bool contains(float x, float y) const;

private:
    sf::RectangleShape button;
    bool visible = false;
};

#endif //BURNBOOKALFA_RETURNBUTTON_H