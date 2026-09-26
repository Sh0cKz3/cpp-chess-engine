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
    const int pawnDirection =
        (byColour == PieceColour::White) ? -1 : 1;

    // Pawns
    for (int columnOffset : {-1, 1}) {
        const int pawnRow = row - pawnDirection;
        const int pawnColumn = column + columnOffset;

        if (pawnRow >= 0 && pawnRow < 8 &&
            pawnColumn >= 0 && pawnColumn < 8) {

            const auto& piece = board.getPiece(pawnRow, pawnColumn);

            if (piece &&
                piece->colour == byColour &&
                piece->type == PieceType::Pawn) {
                return true;
            }
        }
    }

    // Knights
    const int knightMoves[8][2] = {
        {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
        {1, -2}, {1, 2}, {2, -1}, {2, 1}
    };

    for (const auto& offset : knightMoves) {
        const int attackerRow = row + offset[0];
        const int attackerColumn = column + offset[1];

        if (attackerRow < 0 || attackerRow >= 8 ||
            attackerColumn < 0 || attackerColumn >= 8) {
            continue;
        }

        const auto& piece =
            board.getPiece(attackerRow, attackerColumn);

        if (piece &&
            piece->colour == byColour &&
            piece->type == PieceType::Knight) {
            return true;
        }
    }

    // Kings
    const int kingMoves[8][2] = {
        {-1, -1}, {-1, 0}, {-1, 1},
        {0, -1},           {0, 1},
        {1, -1},  {1, 0},  {1, 1}
    };

    for (const auto& offset : kingMoves) {
        const int attackerRow = row + offset[0];
        const int attackerColumn = column + offset[1];

        if (attackerRow < 0 || attackerRow >= 8 ||
            attackerColumn < 0 || attackerColumn >= 8) {
            continue;
        }

        const auto& piece =
            board.getPiece(attackerRow, attackerColumn);

        if (piece &&
            piece->colour == byColour &&
            piece->type == PieceType::King) {
            return true;
        }
    }

    // Sliding pieces
    const int straightDirections[4][2] = {
        {-1, 0},
        {1, 0},
        {0, -1},
        {0, 1}
    };

    for (const auto& direction : straightDirections) {
        int attackerRow = row + direction[0];
        int attackerColumn = column + direction[1];

        while (attackerRow >= 0 && attackerRow < 8 &&
               attackerColumn >= 0 && attackerColumn < 8) {

            const auto& piece =
                board.getPiece(attackerRow, attackerColumn);

            if (piece) {
                if (piece->colour == byColour &&
                    (piece->type == PieceType::Rook ||
                     piece->type == PieceType::Queen)) {
                    return true;
                }

                break;
            }

            attackerRow += direction[0];
            attackerColumn += direction[1];
        }
    }

    const int diagonalDirections[4][2] = {
        {-1, -1},
        {-1, 1},
        {1, -1},
        {1, 1}
    };

    for (const auto& direction : diagonalDirections) {
        int attackerRow = row + direction[0];
        int attackerColumn = column + direction[1];

        while (attackerRow >= 0 && attackerRow < 8 &&
               attackerColumn >= 0 && attackerColumn < 8) {

            const auto& piece =
                board.getPiece(attackerRow, attackerColumn);

            if (piece) {
                if (piece->colour == byColour &&
                    (piece->type == PieceType::Bishop ||
                     piece->type == PieceType::Queen)) {
                    return true;
                }

                break;
            }

            attackerRow += direction[0];
            attackerColumn += direction[1];
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
    const std::vector<Move> legalMoves = GenerateLegalMoves(board, fromRow, fromColumn);

    for (const Move& move : legalMoves) {
        if (move.toRow == toRow && move.toColumn == toColumn) {
            return true;
        }
    }
    return false;
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


void ChessRules::AddSlidingMoves(
    const Board& board,
    std::vector<Move>& legalMoves,
    int fromRow,
    int fromColumn,
    const int directions[4][2],
    const Piece& piece,
    int directionCount
) {
    for (int i = 0; i < directionCount; i++){
        int toRow = fromRow + directions[i][0];
        int toColumn = fromColumn + directions[i][1];

        while (toRow >= 0 && toRow < 8 && toColumn >= 0 && toColumn < 8) {
            const auto& destinationPiece = board.getPiece(toRow, toColumn);

            if (destinationPiece && destinationPiece->colour == piece.colour) {
                break; // Stop if we encounter our own piece
            }

            Move move{fromRow, fromColumn, toRow, toColumn};
            legalMoves.push_back(move);

            if (destinationPiece && destinationPiece->colour != piece.colour) {
                break; // Stop if we capture an opponent's piece
            }

            toRow += directions[i][0];
            toColumn += directions[i][1];
        }
    }
}

std::vector<Move> ChessRules::GeneratePseudoLegalMoves(const Board& board, int fromRow, int fromColumn) {
    std::vector<Move> legalMoves;

    const auto& piece = board.getPiece(fromRow, fromColumn);

    if (!piece || piece->colour != board.GetTurn()) {
        return legalMoves;
    }

    // KNIGHT
    if (piece -> type == PieceType::Knight)
    {
        const int knightMoves[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2}, {1, 2}, {2, -1}, {2, 1}
        }; // the "circle" of 8 knight moves

        for (const auto& offset : knightMoves) {
            int toRow = fromRow + offset[0];
            int toColumn = fromColumn + offset[1];

            if (toRow < 0 || toRow >= 8 || toColumn < 0 || toColumn >= 8) {
                continue; // Skip out-of-bounds moves
            }

            const auto& destinationPiece = board.getPiece(toRow, toColumn);
            
            if(destinationPiece && destinationPiece->colour == piece->colour) {
                continue; // Skip moves that capture own pieces
            }

            Move move{fromRow, fromColumn, toRow, toColumn};
            legalMoves.push_back(move);
        }
    }

    // BISHOP
    else if (piece -> type == PieceType::Bishop){
        int directions[4][2] = {
            {-1, -1}, {-1, 1}, {1, -1}, {1, 1}
        };

        AddSlidingMoves(board, legalMoves, fromRow, fromColumn, directions, *piece, 4);
    }

    // ROOK
    else if (piece -> type == PieceType::Rook){
        int directions[4][2] = {
            {-1, 0}, {1, 0}, {0, -1}, {0, 1}
        };

        AddSlidingMoves(board, legalMoves, fromRow, fromColumn, directions, *piece, 4);
    }

    // QUEEN
    else if (piece -> type == PieceType::Queen){
        int directions[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},          {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        AddSlidingMoves(board, legalMoves, fromRow, fromColumn, directions, *piece, 8);
    }

    // KING
    else if (piece -> type == PieceType::King){
        const int kingMoves[8][2] = {
            {-1, -1}, {-1, 0}, {-1, 1},
            {0, -1},          {0, 1},
            {1, -1}, {1, 0}, {1, 1}
        };

        for (const auto& offset : kingMoves) {
            int toRow = fromRow + offset[0];
            int toColumn = fromColumn + offset[1];

            if (toRow < 0 || toRow >= 8 || toColumn < 0 || toColumn >= 8) {
                continue; // Skip out-of-bounds moves
            }

            const auto& destinationPiece = board.getPiece(toRow, toColumn);
            
            if(destinationPiece && destinationPiece->colour == piece->colour) {
                continue; // Skip moves that capture own pieces
            }

            Move move{fromRow, fromColumn, toRow, toColumn};
            legalMoves.push_back(move);
        }

        // Castling
        if(piece->colour == PieceColour::White && !board.WhiteKingMoved) {
            // White kingside castling
            if (!board.WhiteKingMoved &&
                !board.WhiteKingsideRookMoved &&
                !board.getPiece(7, 5) &&
                !board.getPiece(7, 6))
            {
                const auto& rook = board.getPiece(7, 7);

                if (rook && rook->type == PieceType::Rook && rook->colour == PieceColour::White){
                    legalMoves.push_back(Move{7, 4, 7, 6});
                }
            }

            // White queenside castling
            if(!board.WhiteQueensideRookMoved &&
                !board.getPiece(7, 1) &&
                !board.getPiece(7, 2) &&
                !board.getPiece(7, 3)) {
                
                const auto& rook = board.getPiece(7, 0);

                if (rook && rook->type == PieceType::Rook && rook->colour == PieceColour::White){
                    legalMoves.push_back(Move{7, 4, 7, 2});
                }
            }
        }
        else if(piece->colour == PieceColour::Black && !board.BlackKingMoved) {
            // Black kingside castling
            if(!board.BlackKingsideRookMoved &&
                !board.getPiece(0, 5) &&
                !board.getPiece(0, 6)) {
                const auto& rook = board.getPiece(0, 7);

                if (rook && rook->type == PieceType::Rook && rook->colour == PieceColour::Black){
                    legalMoves.push_back(Move{0, 4, 0, 6});
                }
            }

            // Black queenside castling
            if(!board.BlackQueensideRookMoved &&
                !board.getPiece(0, 1) &&
                !board.getPiece(0, 2) &&
                !board.getPiece(0, 3)) {
                const auto& rook = board.getPiece(0, 0);

                if (rook && rook->type == PieceType::Rook && rook->colour == PieceColour::Black){
                    legalMoves.push_back(Move{0, 4, 0, 2});
                }
            }
        }
    }

    // Pawn
    else if (piece -> type == PieceType::Pawn){
        const int direction = (piece->colour == PieceColour::White) ? -1 : 1;
        int toRow = fromRow + direction;
        
        // Forward move
        if (toRow >= 0 && toRow < 8) {
            const auto& destinationPiece = board.getPiece(toRow, fromColumn);
            if (!destinationPiece) {
                Move move{fromRow, fromColumn, toRow, fromColumn};
                legalMoves.push_back(move);

                // Double move from starting position
                if ((piece->colour == PieceColour::White && fromRow == 6) ||
                    (piece->colour == PieceColour::Black && fromRow == 1)) {
                    int doubleMoveRow = fromRow + 2 * direction;
                    const auto& doubleMoveDestination = board.getPiece(doubleMoveRow, fromColumn);
                    if (!doubleMoveDestination) {
                        Move doubleMove{fromRow, fromColumn, doubleMoveRow, fromColumn};
                        legalMoves.push_back(doubleMove);
                    }
                }
            }
        }

        // Captures
        for (int colOffset : {-1, 1}) {
            int toColumn = fromColumn + colOffset;
            if (toColumn >= 0 && toColumn < 8) {
                const auto& destinationPiece = board.getPiece(toRow, toColumn);
                if (destinationPiece && destinationPiece->colour != piece->colour) {
                    Move move{fromRow, fromColumn, toRow, toColumn};
                    legalMoves.push_back(move);
                }

                // En passant
                if (!destinationPiece &&
                    board.enPassantTarget &&
                    toRow == board.enPassantTarget->first &&
                    toColumn == board.enPassantTarget->second) {

                    const auto& adjacentPiece = board.getPiece(fromRow, toColumn);
                    if (adjacentPiece &&
                        adjacentPiece->type == PieceType::Pawn &&
                        adjacentPiece->colour != piece->colour) {
                        Move enPassantMove{fromRow, fromColumn, toRow, toColumn};
                        legalMoves.push_back(enPassantMove);
                    }
                }
            }
        }
    }


    return legalMoves;
}

std::vector<Move> ChessRules::GenerateLegalMoves(const Board& board, int fromRow, int fromColumn) {
    std::vector<Move> legalMoves;
    const auto& piece = board.getPiece(fromRow, fromColumn);

    const std::vector<Move> pseudoLegalMoves = GeneratePseudoLegalMoves(board, fromRow, fromColumn);

    for (const Move& move : pseudoLegalMoves) {
        // Castling
        if (piece->type == PieceType::King && std::abs(move.toColumn - move.fromColumn) == 2) {
            const int direction = (move.toColumn > move.fromColumn) ? 1 : -1;
            const int crossingColumn = move.fromColumn + direction;
            const PieceColour opponentColour = (board.GetTurn() == PieceColour::White) ? PieceColour::Black : PieceColour::White;

            if (IsSquareAttacked(board, fromRow, fromColumn, opponentColour) ||
                IsSquareAttacked(board, fromRow, crossingColumn, opponentColour) ||
                IsSquareAttacked(board, move.toRow, move.toColumn, opponentColour)) {
                continue; // Skip this castling move as it would put the king in check
            }
        }

        Board tempBoard = board;
        
        // En-Passant check
        if (piece->type == PieceType::Pawn && move.fromColumn != move.toColumn && !board.getPiece(move.toRow, move.toColumn)) {
            tempBoard.squares[move.fromRow][move.toColumn].reset();
        }

        tempBoard.makeMove(move.fromRow, move.fromColumn, move.toRow, move.toColumn);

        if (!IsKingInCheck(tempBoard, board.GetTurn())) { // getTurn from board, guaranteed to be the right colour
            legalMoves.push_back(move);
        }
    }

    return legalMoves;
}

std::vector<Move> ChessRules::GenerateLegalMoves(const Board& board)
{
    std::vector<Move> legalMoves;

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {

            const auto& piece = board.getPiece(row, column);

            if (!piece || piece->colour != board.GetTurn()) {
                continue;
            }

            const std::vector<Move> pieceMoves = GenerateLegalMoves(board, row, column);

            legalMoves.insert(legalMoves.end(), pieceMoves.begin(), pieceMoves.end());
        }
    }

    return legalMoves;
}