#include "Board.hpp"
#include "Piece.hpp"

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

    return true;
}
