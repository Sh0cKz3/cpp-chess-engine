#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>
#include "Board.hpp"
#include "PieceTextures.hpp"

class Renderer {
    private:
        const float squareSize;
        const PieceTextures& pieceTextures;

    public:
        Renderer(const PieceTextures& pieceTextures, float squareSize);

        void draw(
            sf::RenderWindow& window, 
            const Board& board,
            int selectedRow,
            int selectedColumn) const;
};

#endif