#ifndef CHESS_RULLES_HPP
#define CHESS_RULLES_HPP

#include "Board.hpp"
#include "Move.hpp"
#include <vector>

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

    static std::vector<Move> GenerateLegalMoves(const Board& board, int fromRow, int fromColumn); // for checking single move legality (with checks)
    static std::vector<Move> GeneratePseudoLegalMoves(const Board& board, int fromRow, int fromColumn); // for checking single move legality (without checks)

    static std::vector<Move> GenerateLegalMoves(const Board& board); // for generating all legal moves for the current player (engine)

    
private:
    static void AddSlidingMoves(const Board& board, 
        std::vector<Move>& legalMoves, 
        int fromRow, int fromColumn, 
        const int directions[4][2], 
        const Piece& piece,
        int directionCount);
};

#endif