#ifndef BOARD_HPP
#define BOARD_HPP

#include "Piece.hpp"
#include <utility>
#include <optional>
#include <array>

class Board
{

friend class ChessRules;

private:
    std::array<std::array<std::optional<Piece>, 8>, 8> squares;
    std::optional<std::pair<int, int>> enPassantTarget;
    PieceColour turn = PieceColour::White;

    bool WhiteKingMoved = {false};
    bool BlackKingMoved = {false};

    bool WhiteKingsideRookMoved = {false};
    bool WhiteQueensideRookMoved = {false};

    bool BlackKingsideRookMoved = {false};
    bool BlackQueensideRookMoved = {false};

    void makeMove(int fromRow, int fromColumn, int toRow, int toColumn);

public:
    Board();

    const std::optional<Piece>& getPiece(int row, int column) const;

    void movePiece(int fromRow, int fromColumn, int toRow, int toColumn);

    PieceColour getTurn() const;
};

#endif