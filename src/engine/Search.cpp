#include "engine/Search.hpp"
#include "engine/ChessRules.hpp"
#include "engine/Evaluation.hpp"

int Search::SearchPosition(Board& board, int depth, int alpha, int beta) {
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

            const int score = SearchPosition(board, depth - 1, alpha, beta);

            board.UnMakeMove(move, state);

            if (score > bestScore) {
                bestScore = score;
            }

            if (bestScore > alpha) { // if better score found for white, update alpha
                alpha = bestScore;
            }

            if (alpha >= beta) { // if alpha is greater than or equal to beta, prune the remaining branches
                break;
            }
        }
    }
    else {
        bestScore = 1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1, alpha, beta);

            board.UnMakeMove(move, state);

            if (score < bestScore) {
                bestScore = score;
            }

            if (bestScore < beta) { // if better score found for black, update beta
                beta = bestScore;
            }

            if (beta <= alpha) { // if beta is less than or equal to alpha, prune the remaining branches
                break;
            }
        }
    }


    return bestScore;
}

Move Search::FindBestMove(Board& board, int depth) {
    const std::vector<Move> moves = ChessRules::GenerateLegalMoves(board);

    Move bestMove = moves[0];
    int bestScore;

    int alpha = -1000000;
    int beta = 1000000; 

    if (board.GetTurn() == PieceColour::White) {
        bestScore = -1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1, alpha, beta);

            board.UnMakeMove(move, state);

            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }

            if (bestScore > alpha) { // if better score found for white, update alpha
                alpha = bestScore;
            }
        }
    }
    else {
        bestScore = 1000000;

        for (const Move& move : moves) {
            MoveState state = board.MakeMove(move);

            const int score = SearchPosition(board, depth - 1, alpha, beta);

            board.UnMakeMove(move, state);

            if (score < bestScore) {
                bestScore = score;
                bestMove = move;
            }

            if (bestScore < beta) { // if better score found for black, update beta
                beta = bestScore;
            }
        }
    }

    return bestMove;
}