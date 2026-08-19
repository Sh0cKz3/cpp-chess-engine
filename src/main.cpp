#include "Piece.hpp"
#include "Board.hpp"
#include "PieceTextures.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

int main(){
    const int windowSize = 1200;
    const float squareSize = windowSize / 8.0f;
    int selectedRow = -1;
    int selectedColumn = -1;

    Board board;
    PieceTextures pieceTextures;


    sf::RenderWindow window(
        sf::VideoMode({windowSize, windowSize}),
         "Chess Engine"
        );

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

                if (mouseButtonPressedEvent->button == sf::Mouse::Button::Right) {
                    selectedRow = -1;
                    selectedColumn = -1;
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

                const auto& piece = board.getPiece(row, col); // Auto because optional
                if (piece) {
                    const sf::Texture& texture = pieceTextures.getTexture(piece->type, piece->colour);

                    sf::Sprite sprite(texture);

                    const auto textureSize = texture.getSize();

                    const float scaleX = squareSize / textureSize.x;
                    const float scaleY = squareSize / textureSize.y;

                    sprite.setScale({scaleX * 0.9f, scaleY * 0.9f});
                    sprite.setPosition({col * squareSize + (squareSize - squareSize * 0.9f) / 2.0f, row * squareSize + (squareSize - squareSize * 0.9f) / 1.5f}); //center the sprite in the square at 90% size

                    window.draw(sprite);
                }   
            }
        }

        window.display();
    }

    return 0;  
}