#ifndef CORE_MOVE_HPP
#define CORE_MOVE_HPP

#include "core/Piece.hpp"
#include <optional>
#include <utility>

struct Move {
    int fromRow;
    int fromColumn;
    int toRow;
    int toColumn;

    std::optional<PieceType> promotionPiece; // Optional promotion type for pawn promotion
};

struct MoveState{
    std::optional<Piece> capturedPiece;
    std::optional<std::pair<int, int>> enPassantTarget; // Optional because these aren't necessary for any move
    
    PieceColour turn;

    int halfmoveClock;

    bool whiteKingMoved;
    bool blackKingMoved;

    bool whiteKingsideRookMoved;
    bool whiteQueensideRookMoved;

    bool blackKingsideRookMoved;
    bool blackQueensideRookMoved;

    bool wasEnPassant{false};
    bool wasCastling{false};
};

#endif