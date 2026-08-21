#include "Piece.hpp"
#include "Board.hpp"
#include "Renderer.hpp"
#include "PieceTextures.hpp"
#include "ChessRules.hpp"
#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>

int main(){
    const int windowSize = {1000};
    const float squareSize = {windowSize / 8.0f};
    int selectedRow = {-1};
    int selectedColumn = {-1};

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

                    if (board.IsPromotionPending()){
                        const int promotionRow = board.GetPromotionRow();
                        const int promotionColumn = board.GetPromotionColumn();

                        int startColumn;

                        if (promotionColumn <= 4)
                        {
                            startColumn = promotionColumn;
                        }
                        else
                        {
                            startColumn = promotionColumn - 3;
                        }

                        if (row == promotionRow &&
                            column >= startColumn &&
                            column < startColumn + 4)
                        {
                            const int option = column - startColumn;

                            switch (option)
                            {
                                case 0:
                                    board.Promote(PieceType::Queen);
                                    break;

                                case 1:
                                    board.Promote(PieceType::Rook);
                                    break;

                                case 2:
                                    board.Promote(PieceType::Bishop);
                                    break;

                                case 3:
                                    board.Promote(PieceType::Knight);
                                    break;
                            }
                        }

                        continue; // Skip the rest of the loop to avoid processing the promotion click as a regular move
                    }

                    if(selectedRow == -1 && selectedColumn == -1) {
                        const auto& piece = board.getPiece(row, column); // Auto because optional

                        if (piece && piece->colour == board.GetTurn()) {
                            selectedRow = row;
                            selectedColumn = column;
                        }                        
                    }

                    else if (ChessRules::IsLegalMove(board, selectedRow, selectedColumn, row, column)) {
                        board.MovePiece(selectedRow, selectedColumn, row, column);
                        if (ChessRules::IsCheckmate(board, board.GetTurn())) {
                                std::cout << "CHECKMATE!\n";
                            }
                        else if (ChessRules::IsStalemate(board, board.GetTurn())) {
                            std::cout << "STALEMATE!\n";
                            }
                        else if (board.IsFiftyMoveDraw()){
                            std::cout << "50-MOVE DRAW!\n";
                        }

                        selectedRow = -1;
                        selectedColumn = -1;
                    }  
                }

                if (mouseButtonPressedEvent->button == sf::Mouse::Button::Right && !board.IsPromotionPending()) {
                    selectedRow = -1;
                    selectedColumn = -1;
                }
            }
        }

        window.clear();

        renderer.Draw(window, board, selectedRow, selectedColumn);
        if (board.IsPromotionPending()) {
            renderer.DrawPromotionOptions(window, board);
        }

        window.display();
    }

    return 0;  
}