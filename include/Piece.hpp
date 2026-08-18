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

enum class PieceColor
{
    White,
    Black
};

class Piece
{
public:
    PieceType type;
    PieceColor color;
};

#endif