#include "engine/Search.hpp"
#include "engine/ChessRules.hpp"
#include "engine/Evaluation.hpp"

int Search::SearchPosition(Board& board, int depth) {
    if (depth == 0) {
        return Evaluation::EvaluateBoard(board);
    }

    const std::vector<Move> moves = ChessRules::GenerateLegalMoves(board);

    if (moves.empty())
    {
        if (ChessRules::IsKingInCheck(board, board.GetTurn()))
        {
            return (board.GetTurn() == PieceColour::White) // one of the kings is in check and has no moves, checkmate
                ? -1000000
                : 1000000;
        }

        return 0; // stalemate
    }

    int bestScore;

    if (board.GetTurn() == PieceColour::White) {
        bestScore = -1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1);

            board.UnMakeMove(move, state);

            if (score > bestScore) {
                bestScore = score;
            }
        }
    }
    else {
        bestScore = 1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1);

            board.UnMakeMove(move, state);

            if (score < bestScore) {
                bestScore = score;
            }
        }
    }


    return bestScore;
}

Move Search::FindBestMove(Board& board, int depth) {
    const std::vector<Move> moves = ChessRules::GenerateLegalMoves(board);

    Move bestMove = moves[0];
    int bestScore;

    if (board.GetTurn() == PieceColour::White) {
        bestScore = -1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1);

            board.UnMakeMove(move, state);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }
    else {
        bestScore = 1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1);

            board.UnMakeMove(move, state);

            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
    }

    return bestMove;
}