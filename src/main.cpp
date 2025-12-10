#include <SFML/Graphics.hpp>
#include "Board.h"
#include "card.h"
#include <iostream>

using namespace sf;

int main() {

    RenderWindow window(
        VideoMode({1820,1080}),
        "Tedzan uci",
        sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize
    );



    Board board(window.getSize().x, window.getSize().y);

    sf::Texture cardTexture;
    if (!cardTexture.loadFromFile("assets/cards/abortus.jpg"))
    {
        std::cerr << "Greska pri ucitavanju card texture!\n";
    }

    Card testCard(
        cardTexture,
        5, 2, 1,
        Rarity::Legendary,
        AttackType::Chomper,
        "Basic card");

    testCard.setPosition(10, 1);


    while (window.isOpen())
    {
        while (auto event = window.pollEvent())
        {
            // Zatvaranje
            if (event->is<Event::Closed>())
                window.close();

            // Resize
            if (event->is<Event::Resized>())
            {
                auto* e = event->getIf<Event::Resized>();
                board.onResize(e->size.x, e->size.y);

                // Ako želiš da ne rastegne sliku, vec viewport:
                sf::View view(sf::FloatRect(
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(static_cast<float>(e->size.x),
                 static_cast<float>(e->size.y))
));

                window.setView(view);
            }

            // Klik
            if (event->is<Event::MouseButtonPressed>())
            {
                auto* e = event->getIf<Event::MouseButtonPressed>();
                if (e->button == sf::Mouse::Button::Left
)
                {
                    board.handleClick(e->position.x, e->position.y);
                }
            }
        }

        window.clear(Color::White);
        board.draw(window);
        window.draw(testCard.getSprite());

        window.display();
    }

    return 0;
}