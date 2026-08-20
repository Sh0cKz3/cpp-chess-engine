#ifndef BOARD_HPP
#define BOARD_HPP

#include "Piece.hpp"

#include <optional>
#include <array>

class Board
{
private:
    std::array<std::array<std::optional<Piece>, 8>, 8> squares;
    PieceColour turn = PieceColour::White;

    bool WhiteKingMoved = false;
    bool BlackKingMoved = false;

    bool WhiteKingsideRookMoved = false;
    bool WhiteQueensideRookMoved = false;

    bool BlackKingsideRookMoved = false;
    bool BlackQueensideRookMoved = false;

    bool isPathClear(int fromRow, int fromColumn, int toRow, int toColumn) const;

    bool isSquareAttacked(int row, int column, PieceColour byColour) const;

    bool isPseudoLegalMove(int fromRow, int fromColumn, int toRow, int toColumn) const;

    void makeMove(int fromRow, int fromColumn, int toRow, int toColumn);

public:
    Board();

    const std::optional<Piece>& getPiece(int row, int column) const;

    void movePiece(int fromRow, int fromColumn, int toRow, int toColumn);

    PieceColour getTurn() const;

    bool isKingInCheck(PieceColour colour) const;

    bool isLegalMove(int fromRow, int fromColumn, int toRow, int toColumn) const;

};

#endif