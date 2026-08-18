#include "PieceTextures.hpp"

PieceTextures::PieceTextures() {
    // Load textures for each piece type and colour
    textures[static_cast<int>(PieceColour::White)][static_cast<int>(PieceType::Pawn)].loadFromFile("assets/pieces/White_Pawn.png");
    textures[static_cast<int>(PieceColour::White)][static_cast<int>(PieceType::Rook)].loadFromFile("assets/pieces/White_Rook.png");
    textures[static_cast<int>(PieceColour::White)][static_cast<int>(PieceType::Knight)].loadFromFile("assets/pieces/White_Knight.png");
    textures[static_cast<int>(PieceColour::White)][static_cast<int>(PieceType::Bishop)].loadFromFile("assets/pieces/White_Bishop.png");
    textures[static_cast<int>(PieceColour::White)][static_cast<int>(PieceType::Queen)].loadFromFile("assets/pieces/White_Queen.png");
    textures[static_cast<int>(PieceColour::White)][static_cast<int>(PieceType::King)].loadFromFile("assets/pieces/White_King.png");

    textures[static_cast<int>(PieceColour::Black)][static_cast<int>(PieceType::Pawn)].loadFromFile("assets/pieces/Black_Pawn.png");
    textures[static_cast<int>(PieceColour::Black)][static_cast<int>(PieceType::Rook)].loadFromFile("assets/pieces/Black_Rook.png");
    textures[static_cast<int>(PieceColour::Black)][static_cast<int>(PieceType::Knight)].loadFromFile("assets/pieces/Black_Knight.png");
    textures[static_cast<int>(PieceColour::Black)][static_cast<int>(PieceType::Bishop)].loadFromFile("assets/pieces/Black_Bishop.png");
    textures[static_cast<int>(PieceColour::Black)][static_cast<int>(PieceType::Queen)].loadFromFile("assets/pieces/Black_Queen.png");
    textures[static_cast<int>(PieceColour::Black)][static_cast<int>(PieceType::King)].loadFromFile("assets/pieces/Black_King.png");
}

const sf::Texture& PieceTextures::getTexture(PieceType type, PieceColour colour) const{
    const std::size_t colourIndex = static_cast<std::size_t>(colour);
    const std::size_t typeIndex = static_cast<std::size_t>(type);

    return textures[colourIndex][typeIndex];
}