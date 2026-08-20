#include "Renderer.hpp"

Renderer::Renderer(const PieceTextures& pieceTextures, float squareSize) : pieceTextures(pieceTextures), squareSize(squareSize) {    
}

void Renderer::Draw(
    sf::RenderWindow& window, 
    const Board& board,
    int selectedRow,
    int selectedColumn)
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


void Renderer::DrawPromotionOptions(
    sf::RenderWindow& window,
    const Board& board
)
{
    const int row = board.GetPromotionRow();
    const int column = board.GetPromotionColumn();
    const PieceColour colour = board.GetPromotionColour();

    const PieceType promotionPieces[4] = {
        PieceType::Queen,    // 0 
        PieceType::Rook,     // 1
        PieceType::Bishop,   // 2
        PieceType::Knight    // 3
    }; 

    int startColumn;

    if (column <= 4)
    {
        startColumn = column;
    }
    else
    {
        startColumn = column - 3;
    }

    for (int i = 0; i < 4; ++i)
    {
        const int optionColumn = startColumn + i;

        sf::RectangleShape option({squareSize, squareSize});

        option.setPosition({
            optionColumn * squareSize,
            row * squareSize
        });

        option.setFillColor(sf::Color::White);

        window.draw(option);

        const auto& texture =
            pieceTextures.getTexture(
                promotionPieces[i],
                colour
            );

        sf::Sprite sprite(texture);

        const auto textureSize = texture.getSize();

        const float scaleX =
            squareSize / textureSize.x;

        const float scaleY =
            squareSize / textureSize.y;

        sprite.setScale({
            scaleX * 0.9f,
            scaleY * 0.9f
        });

        sprite.setPosition({
            optionColumn * squareSize +
                (squareSize - squareSize * 0.9f) / 2.0f,

            row * squareSize +
                (squareSize - squareSize * 0.9f) / 1.5f
        });

        window.draw(sprite);
    }
}