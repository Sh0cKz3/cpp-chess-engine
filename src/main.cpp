#include "Piece.hpp"
#include "Board.hpp"
#include "Renderer.hpp"
#include "PieceTextures.hpp"
#include <SFML/Graphics.hpp>
#include <optional>

int main(){
    const int windowSize = 1000;
    const float squareSize = windowSize / 8.0f;
    int selectedRow = -1;
    int selectedColumn = -1;

    Board board;
    PieceTextures pieceTextures;
    Renderer renderer(pieceTextures, squareSize);


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

                    const int column = static_cast<int>(mousePosition.x / squareSize); // Explicitly convert to int
                    const int row = static_cast<int>(mousePosition.y / squareSize);
                    const auto& destinationPiece = board.getPiece(row, column);

                    if(selectedRow == -1 && selectedColumn == -1) {
                        const auto& piece = board.getPiece(row, column); // Auto because optional

                        if (piece && piece->colour == board.getTurn()) {
                            selectedRow = row;
                            selectedColumn = column;
                        }                        
                    }

                    else if (!destinationPiece || destinationPiece->colour != board.getTurn()) {
                        board.movePiece(selectedRow, selectedColumn, row, column);
                        selectedRow = -1;
                        selectedColumn = -1;
                    }  
                }

                if (mouseButtonPressedEvent->button == sf::Mouse::Button::Right) {
                    selectedRow = -1;
                    selectedColumn = -1;
                }
            }
        }

        window.clear();
        renderer.draw(window, board, selectedRow, selectedColumn);
        window.display();
    }

    return 0;  
}