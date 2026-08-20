#ifndef CHESS_RULLES_HPP
#define CHESS_RULLES_HPP

#include "Board.hpp"

class ChessRules
{
public:
    static bool IsPathClear(
        const Board& board,
        int fromRow,
        int fromColumn,
        int toRow,
        int toColumn
    );

    static bool IsPseudoLegalMove(
        const Board& board,
        int fromRow,
        int fromColumn,
        int toRow,
        int toColumn
    );

    static bool IsSquareAttacked(
        const Board& board,
        int row,
        int column,
        PieceColour byColour
    );

    static bool IsKingInCheck(
        const Board& board,
        PieceColour colour
    );

    static bool IsLegalMove(
        const Board& board,
        int fromRow,
        int fromColumn,
        int toRow,
        int toColumn
    );

    static bool HasLegalMoves(const Board& board, PieceColour colour);

    static bool IsCheckmate(const Board& board, PieceColour colour);

    static bool IsStalemate(const Board& board, PieceColour colour);

};

#endif