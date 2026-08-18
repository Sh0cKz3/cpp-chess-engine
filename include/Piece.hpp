#ifndef PIECE_HPP
#define PIECE_HPP

enum class PieceType
{
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class PieceColour
{
    White,
    Black
};

class Piece
{
public:
    Piece(PieceType type, PieceColour colour);

    PieceType type;
    PieceColour colour;
};

#endif