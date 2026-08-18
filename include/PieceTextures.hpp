#ifndef PIECE_TEXTURES_HPP
#define PIECE_TEXTURES_HPP

#include "Piece.hpp"
#include <SFML/Graphics.hpp>
#include <array>

class PieceTextures
{
private:
    std::array<std::array<sf::Texture, 6>, 2> textures; // 2 colours, 6 piece types

public:
    PieceTextures();

    const sf::Texture& getTexture(PieceType type, PieceColour colour) const;
};

#endif