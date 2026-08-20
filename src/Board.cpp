#include "Board.hpp"
#include "Piece.hpp"
#include <cstdlib>

const std::optional<Piece>& Board::getPiece(int row, int column) const {
    return squares[row][column];
}

const PieceType backRank[8] =
{
    PieceType::Rook,
    PieceType::Knight,
    PieceType::Bishop,
    PieceType::Queen,
    PieceType::King,
    PieceType::Bishop,
    PieceType::Knight,
    PieceType::Rook
};

// initialize the board with pieces in their starting positions
Board::Board() {
    for (int column = 0; column < 8; ++column){
    squares[1][column] = Piece(PieceType::Pawn, PieceColour::Black);
    squares[6][column] = Piece(PieceType::Pawn, PieceColour::White);
    }   

    for (int column = 0; column < 8; ++column){
    squares[0][column] = Piece(backRank[column], PieceColour::Black);
    squares[7][column] = Piece(backRank[column], PieceColour::White);
    }
}

void Board::makeMove(int fromRow, int fromColumn, int toRow, int toColumn) {
    squares[toRow][toColumn] = squares[fromRow][fromColumn];
    squares[fromRow][fromColumn].reset();
}

void Board::movePiece(int fromRow, int fromColumn, int toRow, int toColumn)
{
    const auto movingPiece = squares[fromRow][fromColumn];

    // Check if a rook is about to be captured (for castling rights)
    const auto capturedPiece = squares[toRow][toColumn];

    if (capturedPiece && capturedPiece->type == PieceType::Rook)
    {
        if (capturedPiece->colour == PieceColour::White)
        {
            if (toRow == 7 && toColumn == 0)
            {
                WhiteQueensideRookMoved = true;
            }

            if (toRow == 7 && toColumn == 7)
            {
                WhiteKingsideRookMoved = true;
            }
        }
        else
        {
            if (toRow == 0 && toColumn == 0)
            {
                BlackQueensideRookMoved = true;
            }

            if (toRow == 0 && toColumn == 7)
            {
                BlackKingsideRookMoved = true;
            }
        }
    }

    // Check for en passant
    bool isEnPassant = false;

    if (movingPiece && movingPiece->type == PieceType::Pawn)
    {
        const bool movesDiagonally =
            std::abs(fromColumn - toColumn) == 1;

        const bool destinationEmpty =
            !squares[toRow][toColumn];

        const bool targetMatches =
            enPassantTarget &&
            toRow == enPassantTarget->first &&
            toColumn == enPassantTarget->second;

        isEnPassant = movesDiagonally &&
                      destinationEmpty &&
                      targetMatches;
    }

    makeMove(fromRow, fromColumn, toRow, toColumn);

    // Remove the captured pawn during en passant
    if (isEnPassant)
    {
        squares[fromRow][toColumn].reset();
    }

    // Castling
    const auto& movedPiece = squares[toRow][toColumn];

    if (movedPiece &&
        movedPiece->type == PieceType::King &&
        std::abs(toColumn - fromColumn) == 2)
    {
        if (toColumn > fromColumn)
        {
            // Kingside
            makeMove(fromRow, 7, toRow, 5);
        }
        else
        {
            // Queenside
            makeMove(fromRow, 0, toRow, 3);
        }
    }

    // Update en passant target
    if (movingPiece &&
        movingPiece->type == PieceType::Pawn &&
        std::abs(toRow - fromRow) == 2)
    {
        enPassantTarget = {
            (fromRow + toRow) / 2,
            fromColumn
        };
    }
    else
    {
        enPassantTarget.reset();
    }

    // Update castling rights
    if (movingPiece)
    {
        if (movingPiece->type == PieceType::King)
        {
            if (movingPiece->colour == PieceColour::White)
            {
                WhiteKingMoved = true;
            }
            else
            {
                BlackKingMoved = true;
            }
        }

        if (movingPiece->type == PieceType::Rook)
        {
            if (movingPiece->colour == PieceColour::White)
            {
                if (fromRow == 7 && fromColumn == 0)
                {
                    WhiteQueensideRookMoved = true;
                }

                if (fromRow == 7 && fromColumn == 7)
                {
                    WhiteKingsideRookMoved = true;
                }
            }
            else
            {
                if (fromRow == 0 && fromColumn == 0)
                {
                    BlackQueensideRookMoved = true;
                }

                if (fromRow == 0 && fromColumn == 7)
                {
                    BlackKingsideRookMoved = true;
                }
            }
        }
    }

    // Change turn
    if (turn == PieceColour::White)
    {
        turn = PieceColour::Black;
    }
    else
    {
        turn = PieceColour::White;
    }
}

PieceColour Board::getTurn() const {
    return turn;
}

bool Board::isPathClear(int fromRow, int fromColumn, int toRow, int toColumn) const {
    int rowStep = (toRow > fromRow) ? 1 : (toRow < fromRow) ? -1 : 0; // Determine the step for row movement
    int colStep = (toColumn > fromColumn) ? 1 : (toColumn < fromColumn) ? -1 : 0; // Determine the step for column movement

    int currentRow = fromRow + rowStep;
    int currentCol = fromColumn + colStep;

    while (currentRow != toRow || currentCol != toColumn) {
        if (squares[currentRow][currentCol].has_value()) { // Check if the square is occupied
            return false;
        }
        currentRow += rowStep;
        currentCol += colStep;
    }

    return true;
}

bool Board::isLegalMove(int fromRow,int fromColumn,int toRow,int toColumn) const{
    if (!isPseudoLegalMove(fromRow, fromColumn, toRow, toColumn))
    {
        return false;
    }

    const PieceColour movingColour = squares[fromRow][fromColumn]->colour;

    const PieceColour opponent = (movingColour == PieceColour::White) ? PieceColour::Black : PieceColour::White;

    // Castling-specific checks
    if (squares[fromRow][fromColumn]->type == PieceType::King &&
        std::abs(toColumn - fromColumn) == 2)
    {
        // is king in check before castling?
        if (isSquareAttacked(fromRow, fromColumn, opponent))
        {
            return false;
        }

        // is the square the king passes through attacked?
        const int direction =
            (toColumn > fromColumn) ? 1 : -1;

        const int intermediateColumn =
            fromColumn + direction;

        if (isSquareAttacked(
                fromRow,
                intermediateColumn,
                opponent))
        {
            return false;
        }
    }

    Board testBoard = *this;

    testBoard.makeMove(fromRow,fromColumn,toRow,toColumn);

    // is the king in check after the move?
    if (testBoard.isKingInCheck(movingColour))
    {
        return false;
    }

    return true;
}

bool Board::isPseudoLegalMove(int fromRow, int fromColumn, int toRow, int toColumn) const {
    const auto& piece = squares[fromRow][fromColumn];
    const auto& destinationPiece = squares[toRow][toColumn];

    if (!piece) {
        return false;
    }

    if (fromRow == toRow && fromColumn == toColumn) {
        return false;
    }

    switch (piece->type) {
        case PieceType::Pawn: {
            int direction = (piece->colour == PieceColour::White) ? -1 : 1; // White pawns move up (decreasing row), Black pawns move down (increasing row)
            if (fromColumn == toColumn) {
                // check for one-square forward move
                if (toRow == fromRow + direction && !destinationPiece) {
                    return true;
                }
                if ((fromRow == 1 && piece->colour == PieceColour::Black) || 
                    (fromRow == 6 && piece->colour == PieceColour::White)) {
                    // Check for two-square initial move
                    if (toRow == fromRow + 2 * direction && !destinationPiece && !squares[fromRow + direction][fromColumn]) {
                        return true;
                    }
                }
            // check for diagonal capture
            } else if (std::abs(fromColumn - toColumn) == 1 && toRow == fromRow + direction) {
                if (destinationPiece && destinationPiece->colour != piece->colour) {
                    return true;
                }
            }

            // en passant capture
            if (!destinationPiece && enPassantTarget && toRow == enPassantTarget->first && toColumn == enPassantTarget->second) {
                return true;
            }
            return false;
        }

        case PieceType::Knight: {
            int rowDiff = std::abs(toRow - fromRow);
            int colDiff = std::abs(toColumn - fromColumn);
            return (rowDiff == 2 && colDiff == 1) || (rowDiff == 1 && colDiff == 2); // L-shaped move
        }

        case PieceType::Bishop: {
            const int rowDiff = std::abs(toRow - fromRow);
            const int colDiff = std::abs(toColumn - fromColumn);

            if (rowDiff != colDiff) {
                return false; // Not moving diagonally
            }
            return isPathClear(fromRow, fromColumn, toRow, toColumn);
        }

        case PieceType::Rook: {
            if (fromRow != toRow && fromColumn != toColumn) {
                return false; // Not moving in a straight line
            }
            return isPathClear(fromRow, fromColumn, toRow, toColumn);
        }

        case PieceType::Queen: {
            const int rowDiff = std::abs(toRow - fromRow);
            const int colDiff = std::abs(toColumn - fromColumn);

            bool diagonalMove = (rowDiff == colDiff);
            bool straightMove = (fromRow == toRow || fromColumn == toColumn);

            if (!diagonalMove && !straightMove) {
                return false; // Not moving in a straight line or diagonally
            }
            return isPathClear(fromRow, fromColumn, toRow, toColumn);
        }

        case PieceType::King: {
    const int rowDiff = std::abs(toRow - fromRow);
    const int colDiff = std::abs(toColumn - fromColumn);


    if (rowDiff <= 1 && colDiff <= 1 && (rowDiff || colDiff)) {
        return true;
    }

    // Castling
    if (rowDiff == 0 && colDiff == 2) {
        if (piece->colour == PieceColour::White) {
            if (WhiteKingMoved) {
                return false;
            }

            // Kingside: e1 -> g1
            if (toColumn == 6 &&
                !WhiteKingsideRookMoved &&
                squares[7][7] &&
                squares[7][7]->type == PieceType::Rook &&
                squares[7][7]->colour == PieceColour::White &&
                !squares[7][5] &&
                !squares[7][6]) {
                return true;
            }

            // Queenside: e1 -> c1
            if (toColumn == 2 &&
                !WhiteQueensideRookMoved &&
                squares[7][0] &&
                squares[7][0]->type == PieceType::Rook &&
                squares[7][0]->colour == PieceColour::White &&
                !squares[7][1] &&
                !squares[7][2] &&
                !squares[7][3]) {
                return true;
            }
        }
        else {
            if (BlackKingMoved) {
                return false;
            }

            // Kingside: e8 -> g8
            if (toColumn == 6 &&
                !BlackKingsideRookMoved &&
                squares[0][7] &&
                squares[0][7]->type == PieceType::Rook &&
                squares[0][7]->colour == PieceColour::Black &&
                !squares[0][5] &&
                !squares[0][6]) {
                return true;
            }

            // Queenside: e8 -> c8
            if (toColumn == 2 &&
                !BlackQueensideRookMoved &&
                squares[0][0] &&
                squares[0][0]->type == PieceType::Rook &&
                squares[0][0]->colour == PieceColour::Black &&
                !squares[0][1] &&
                !squares[0][2] &&
                !squares[0][3]) {
                return true;
            }
        }
    }

    return false;
}
    }

    return false; // Default return value if no valid move is found
}

bool Board::isSquareAttacked(int row, int column, PieceColour byColour) const {
    for (int r = 0; r < 8; ++r) {
        for (int c = 0; c < 8; ++c) {
            const auto& piece = squares[r][c];
            if (!piece || piece->colour != byColour) {
                continue;
            }

            if (piece->type == PieceType::Pawn) {
                const int direction = (byColour == PieceColour::White) ? -1 : 1;

                if ((row == r + direction) && std::abs(column - c) == 1) {
                    return true; // Pawn attacks diagonally ONLY, hence special handling
                }
            } else if (isPseudoLegalMove(r, c, row, column)) {
                return true; // Other pieces can attack if they can move to the square generally
            }
        }
    }
    return false;
}

bool Board::isKingInCheck(PieceColour colour) const {
    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {
            const auto& piece = squares[row][column];

            if (piece && piece->type == PieceType::King && piece->colour == colour) {
                const PieceColour opponent = (colour == PieceColour::White) ? PieceColour::Black : PieceColour::White;
                return isSquareAttacked(row, column, opponent);
            }
        }
    }
    return false;
}
