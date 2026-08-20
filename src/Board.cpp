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
