#ifndef UI_RENDERER_HPP
#define UI_RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "core/Board.hpp"
#include "ui/PieceTextures.hpp"

class Renderer {
    private:
        const float squareSize;
        const PieceTextures& pieceTextures;

    public:
        Renderer(const PieceTextures& pieceTextures, float squareSize);

        void Draw(
            sf::RenderWindow& window, 
            const Board& board,
            int selectedRow,
            int selectedColumn
        );

        void DrawPromotionOptions(
            sf::RenderWindow& window,
            const Board& board
        );
};

#endif