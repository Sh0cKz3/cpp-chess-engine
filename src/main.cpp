#include "Piece.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

int main(){
    sf::RenderWindow window(
        sf::VideoMode({800, 800}),
         "Chess Engine"
        );

    const float squareSize = 100.0f;
    int selectedRow = -1;
    int selectedColumn = -1;

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouseButtonPressedEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseButtonPressedEvent->button == sf::Mouse::Button::Left) {
                    const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);  

                    const int column = static_cast<int>(mousePosition.x / squareSize); // explicitly convert to int
                    const int row = static_cast<int>(mousePosition.y / squareSize);

                    selectedRow = row;
                    selectedColumn = column;
                }
            }
        }

        window.clear();

        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                sf::RectangleShape square({squareSize, squareSize});
                square.setPosition({col * squareSize, row * squareSize});


                if (row == selectedRow && col == selectedColumn) {
                    square.setFillColor(sf::Color::Green); // Highlight selected square
                }
                else if ((row + col) % 2 == 0) {
                    square.setFillColor(sf::Color(240, 217, 181)); // Light color);
                } 
                else {
                    square.setFillColor(sf::Color(139, 69, 19)); // Dark color);
                }

                window.draw(square);
            }
        }

        window.display();
    }

    return 0;  
}