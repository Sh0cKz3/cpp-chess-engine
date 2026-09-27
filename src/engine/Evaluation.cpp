#include "engine/Evaluation.hpp"

namespace{
    constexpr int PawnValue = 100;
    constexpr int KnightValue = 320;
    constexpr int BishopValue = 330;
    constexpr int RookValue = 500;
    constexpr int QueenValue = 900;
}

int Evaluation::EvaluateBoard(const Board& board) { // literally just go over the board summing piece values
    int score = 0;

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            const auto& piece = board.getPiece(row, column);

            if (!piece) {
                continue;
            }

            int pieceValue = 0;

            switch (piece->type) {
                case PieceType::Pawn:
                    pieceValue = PawnValue;
                    break;
                case PieceType::Knight:
                    pieceValue = KnightValue;
                    break;
                case PieceType::Bishop:
                    pieceValue = BishopValue;
                    break;
                case PieceType::Rook:
                    pieceValue = RookValue;
                    break;
                case PieceType::Queen:
                    pieceValue = QueenValue;
                    break;
                case PieceType::King:
                    break;
            }

            if (piece->colour == PieceColour::White) {
                score += pieceValue; // so + for better white position
            } else {
                score -= pieceValue; // - for better black position
            }
        }
    }

    return score;
}

