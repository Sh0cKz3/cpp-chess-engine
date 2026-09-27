#include "engine/Search.hpp"
#include "engine/ChessRules.hpp"
#include "engine/Evaluation.hpp"

int Search::SearchPosition(Board& board, int depth) {
    if (depth == 0) {
        return Evaluation::EvaluateBoard(board);
    }

    const std::vector<Move> moves = ChessRules::GenerateLegalMoves(board);

    int bestScore = 0;

    for (const Move& move : moves) {
        MoveState state = board.MakeMove(move);
        const int score = SearchPosition(board, depth - 1);
        board.UnMakeMove(move, state);

        if (score > bestScore) {
            bestScore = score;
        }
    }

    return bestScore;
}