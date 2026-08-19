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

void Board::movePiece(int fromRow, int fromColumn, int toRow, int toColumn) {
    squares[toRow][toColumn] = squares[fromRow][fromColumn];
    squares[fromRow][fromColumn].reset();

    if (turn == PieceColour::White) {
        turn = PieceColour::Black;
    } 
    else {
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

bool Board::isLegalMove(int fromRow, int fromColumn, int toRow, int toColumn) const {
    const auto& piece = squares[fromRow][fromColumn];
    const auto& destinationPiece = squares[toRow][toColumn];

    if (!piece) {
        return false;
    }

    if (destinationPiece && 
        destinationPiece->colour == piece->colour) {
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
            // check for en-passant
            } else if (std::abs(fromColumn - toColumn) == 1 && toRow == fromRow + direction) {
                if (destinationPiece && destinationPiece->colour != piece->colour) {
                    return true;
                }
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

            return (rowDiff <= 1 && colDiff <= 1 && (rowDiff || colDiff)); // King can move one square in any direction
        }
    }

    return true;
}
