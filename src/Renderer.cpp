#include "Renderer.hpp"

Renderer::Renderer(const PieceTextures& pieceTextures, float squareSize) : pieceTextures(pieceTextures), squareSize(squareSize) {    
}

void Renderer::draw(
    sf::RenderWindow& window, 
    const Board& board,
    int selectedRow,
    int selectedColumn) const
{
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            sf::RectangleShape square({squareSize, squareSize});
            square.setPosition({col * squareSize, row * squareSize});

            if (row == selectedRow && col == selectedColumn) {
                square.setFillColor(sf::Color::Green); // Highlight selected square
            }
            else if ((row + col) % 2 == 0) {
                square.setFillColor(sf::Color(240, 217, 181)); // Light color
            } 
            else {
                square.setFillColor(sf::Color(139, 69, 19)); // Dark color
            }

            window.draw(square);

            const auto& piece = board.getPiece(row, col);

            if (piece) {
                const auto& texture = pieceTextures.getTexture(piece->type, piece->colour);
                sf::Sprite sprite(texture);

                const auto textureSize = texture.getSize();
                const float scaleX = squareSize / textureSize.x;
                const float scaleY = squareSize / textureSize.y;

                sprite.setScale({scaleX * 0.9f, scaleY * 0.9f});
                sprite.setPosition({
                    col * squareSize + (squareSize - squareSize * 0.9f) / 2.0f,
                    row * squareSize + (squareSize - squareSize * 0.9f) / 1.5f
                }); // Center the sprite in the square at 90% size

                window.draw(sprite);
            }   
        }
    }
}