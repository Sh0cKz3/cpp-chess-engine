#include "ChessRules.hpp"
#include <cstdlib>

bool ChessRules::IsPathClear(
    const Board& board,
    int fromRow,
    int fromColumn,
    int toRow,
    int toColumn
) {int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0;
    int colStep = (toColumn > fromColumn) ? 1 : (toColumn < fromColumn) ? -1 : 0;

    int currentRow = fromRow + rowStep;
    int currentCol = fromColumn + colStep;

    while (currentRow != toRow || currentCol != toColumn) {
        if (board.getPiece(currentRow, currentCol).has_value()) {
            return false;
        }
        currentRow += rowStep;
        currentCol += colStep;
    }
    return true;
}

bool ChessRules::IsSquareAttacked(
    const Board& board,
    int row,
    int column,
    PieceColour byColour
) {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const auto& piece = board.getPiece(r, c);

            if (!piece || piece->colour != byColour) {
                continue;
            }

            if (piece->type == PieceType::Pawn) {
                const int direction =
                    (byColour == PieceColour::White) ? -1 : 1;

                if (row == r + direction &&
                    std::abs(column - c) == 1) {
                    return true;
                }
            }
            else if (IsPseudoLegalMove(board, r, c, row, column)) {
                return true;
            }
        }
    }

    return false;
}

bool ChessRules::IsKingInCheck(
    const Board& board,
    PieceColour colour
) {
    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            const auto& piece = board.getPiece(row, column);

            if (piece &&
                piece->type == PieceType::King &&
                piece->colour == colour) {

                const PieceColour opponent =
                    (colour == PieceColour::White)
                        ? PieceColour::Black
                        : PieceColour::White;

                return IsSquareAttacked(
                    board,
                    row,
                    column,
                    opponent
                );
            }
        }
    }

    return false;
}

bool ChessRules::IsLegalMove(
    const Board& board,
    int fromRow,
    int fromColumn,
    int toRow,
    int toColumn
) {
    if (!IsPseudoLegalMove(
            board,
            fromRow,
            fromColumn,
            toRow,
            toColumn)) {
        return false;
    }

    const auto& movingPiece = board.getPiece(fromRow, fromColumn);

    if (!movingPiece) {
        return false;
    }

    const PieceColour movingColour = movingPiece->colour;

    // Castling
    if (movingPiece->type == PieceType::King &&
        std::abs(toColumn - fromColumn) == 2) {

        const int direction = (toColumn > fromColumn) ? 1 : -1;

        const int middleColumn = fromColumn + direction;

        if (IsSquareAttacked(
                board,
                fromRow,
                fromColumn,
                (movingColour == PieceColour::White)
                    ? PieceColour::Black
                    : PieceColour::White)) {
            return false;
        }

        if (IsSquareAttacked(
                board,
                fromRow,
                middleColumn,
                (movingColour == PieceColour::White)
                    ? PieceColour::Black
                    : PieceColour::White)) {
            return false;
        }
    }

    Board testBoard = board;

    testBoard.makeMove(
        fromRow,
        fromColumn,
        toRow,
        toColumn
    );

    return !IsKingInCheck(testBoard, movingColour);
}

bool ChessRules::HasLegalMoves(
    const Board& board,
    PieceColour colour
) {
    for (int fromRow = 0; fromRow < 8; ++fromRow) {
        for (int fromColumn = 0; fromColumn < 8; ++fromColumn) {

            const auto& piece = board.getPiece(fromRow, fromColumn);

            if (!piece || piece->colour != colour) {
                continue;
            }

            for (int toRow = 0; toRow < 8; ++toRow) {
                for (int toColumn = 0; toColumn < 8; ++toColumn) {

                    if (IsLegalMove(
                            board,
                            fromRow,
                            fromColumn,
                            toRow,
                            toColumn)) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool ChessRules::IsCheckmate(
    const Board& board,
    PieceColour colour
) {
    return IsKingInCheck(board, colour) &&
           !HasLegalMoves(board, colour);
}

bool ChessRules::IsStalemate(
    const Board& board,
    PieceColour colour
) {
    return !IsKingInCheck(board, colour) &&
           !HasLegalMoves(board, colour);
}


bool ChessRules::IsPseudoLegalMove(
    const Board& board,
    int fromRow,
    int fromColumn,
    int toRow,
    int toColumn
) {
    const auto& piece = board.getPiece(fromRow, fromColumn);
    const auto& destinationPiece = board.getPiece(toRow, toColumn);

    if (!piece) {
        return false;
    }

    if (fromRow == toRow && fromColumn == toColumn) {
        return false;
    }

    switch (piece->type) {
        case PieceType::Pawn: {
        int direction = (piece->colour == PieceColour::White) ? -1 : 1;

        if (fromColumn == toColumn) {
            if (toRow == fromRow + direction && !destinationPiece) {
                return true;
            }

            if ((fromRow == 1 && piece->colour == PieceColour::Black) ||
                (fromRow == 6 && piece->colour == PieceColour::White)) {

                if (toRow == fromRow + 2 * direction &&
                    !destinationPiece &&
                    !board.getPiece(fromRow + direction, fromColumn)) {
                    return true;
                }
            }
        }
        else if (std::abs(fromColumn - toColumn) == 1 && toRow == fromRow + direction) {

            // Normal capture
            if (destinationPiece &&
                destinationPiece->colour != piece->colour) {
                return true;
            }

            // En passant
            if (!destinationPiece &&
                board.enPassantTarget &&
                toRow == board.enPassantTarget->first &&
                toColumn == board.enPassantTarget->second) {

                const auto& adjacentPiece =
                    board.getPiece(fromRow, toColumn);

                if (adjacentPiece &&
                    adjacentPiece->type == PieceType::Pawn &&
                    adjacentPiece->colour != piece->colour) {
                    return true;
                }
            }
        }

    return false;
}

        case PieceType::Knight: {
            int rowDiff = std::abs(toRow - fromRow);
            int colDiff = std::abs(toColumn - fromColumn);

            return (rowDiff == 2 && colDiff == 1) ||
                   (rowDiff == 1 && colDiff == 2);
        }

        case PieceType::Bishop: {
            const int rowDiff = std::abs(toRow - fromRow);
            const int colDiff = std::abs(toColumn - fromColumn);

            if (rowDiff != colDiff) {
                return false;
            }

            return IsPathClear(
                board,
                fromRow,
                fromColumn,
                toRow,
                toColumn
            );
        }

        case PieceType::Rook: {
            if (fromRow != toRow && fromColumn != toColumn) {
                return false;
            }

            return IsPathClear(
                board,
                fromRow,
                fromColumn,
                toRow,
                toColumn
            );
        }

        case PieceType::Queen: {
            const int rowDiff = std::abs(toRow - fromRow);
            const int colDiff = std::abs(toColumn - fromColumn);

            bool diagonalMove = (rowDiff == colDiff);
            bool straightMove = (fromRow == toRow || fromColumn == toColumn);

            if (!diagonalMove && !straightMove) {
                return false;
            }

            return IsPathClear(
                board,
                fromRow,
                fromColumn,
                toRow,
                toColumn
            );
        }

        case PieceType::King: {
            const int rowDiff = std::abs(toRow - fromRow);
            const int colDiff = std::abs(toColumn - fromColumn);

            if (rowDiff <= 1 &&
                colDiff <= 1 &&
                (rowDiff || colDiff)) {
                return true;
            }

            // Castling
            if (rowDiff == 0 && colDiff == 2) {

                if (piece->colour == PieceColour::White) {

                    if (board.WhiteKingMoved) {
                        return false;
                    }

                    // Kingside white castling
                    if (toColumn == 6 &&
                        !board.WhiteKingsideRookMoved &&
                        board.getPiece(7, 7) &&
                        board.getPiece(7, 7)->type == PieceType::Rook &&
                        board.getPiece(7, 7)->colour == PieceColour::White &&
                        !board.getPiece(7, 5) &&
                        !board.getPiece(7, 6)) {
                        return true;
                    }

                    // Queenside white castling
                    if (toColumn == 2 &&
                        !board.WhiteQueensideRookMoved &&
                        board.getPiece(7, 0) &&
                        board.getPiece(7, 0)->type == PieceType::Rook &&
                        board.getPiece(7, 0)->colour == PieceColour::White &&
                        !board.getPiece(7, 1) &&
                        !board.getPiece(7, 2) &&
                        !board.getPiece(7, 3)) {
                        return true;
                    }
                }

                else {

                    if (board.BlackKingMoved) {
                        return false;
                    }

                    // Kingside black castling
                    if (toColumn == 6 &&
                        !board.BlackKingsideRookMoved &&
                        board.getPiece(0, 7) &&
                        board.getPiece(0, 7)->type == PieceType::Rook &&
                        board.getPiece(0, 7)->colour == PieceColour::Black &&
                        !board.getPiece(0, 5) &&
                        !board.getPiece(0, 6)) {
                        return true;
                    }

                    // Queenside black castling
                    if (toColumn == 2 &&
                        !board.BlackQueensideRookMoved &&
                        board.getPiece(0, 0) &&
                        board.getPiece(0, 0)->type == PieceType::Rook &&
                        board.getPiece(0, 0)->colour == PieceColour::Black &&
                        !board.getPiece(0, 1) &&
                        !board.getPiece(0, 2) &&
                        !board.getPiece(0, 3)) {
                        return true;
                    }
                }
            }

            return false;
        }
    }

    return false;
}

