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

    int HalfmoveClock = {0};

    PieceColour PromotionColour;
    bool PromotionPending = {false};
    int PromotionRow = {-1};
    int PromotionColumn = {-1};
    

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

    void MovePiece(
        int fromRow, 
        int fromColumn, 
        int toRow, 
        int toColumn
    );

    PieceColour GetTurn() const;

    bool IsPromotionPending() const;
    PieceColour GetPromotionColour() const;
    int GetPromotionRow() const;
    int GetPromotionColumn() const;
    void Promote(PieceType type);

    int GetHalfmoveClock() const;
    bool IsFiftyMoveDraw() const;
};

#endif