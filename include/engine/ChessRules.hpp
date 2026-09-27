#ifndef CORE_CHESS_RULES_HPP
#define CORE_CHESS_RULES_HPP

#include "core/Board.hpp"
#include "core/Move.hpp"
#include <vector>

class ChessRules
{
public:
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
        Board& board,
        int fromRow,
        int fromColumn,
        int toRow,
        int toColumn
    );

    static bool HasLegalMoves(Board& board, PieceColour colour);

    static bool IsCheckmate(Board& board, PieceColour colour);

    static bool IsStalemate(Board& board, PieceColour colour);

    static std::vector<Move> GenerateLegalMoves(Board& board, int fromRow, int fromColumn); // for checking single move legality (with checks)
    static std::vector<Move> GeneratePseudoLegalMoves(const Board& board, int fromRow, int fromColumn); // for checking single move legality (without checks)

    static std::vector<Move> GenerateLegalMoves(Board& board); // for generating all legal moves for the current player (engine)

    
private:
    static void AddSlidingMoves(const Board& board, 
        std::vector<Move>& legalMoves, 
        int fromRow, int fromColumn, 
        const int directions[4][2], 
        const Piece& piece,
        int directionCount);
};

#endif