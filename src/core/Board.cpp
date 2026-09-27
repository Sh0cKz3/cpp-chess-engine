#include "core/Board.hpp"
#include "core/Piece.hpp"
#include <cstdlib>

const std::optional<Piece>& Board::getPiece(int row, int column) const {
    return squares[row][column];
}

bool Board::IsPromotionPending() const {return PromotionPending;}
PieceColour Board::GetPromotionColour() const {return PromotionColour;}
int Board::GetPromotionRow() const {return PromotionRow;}
int Board::GetPromotionColumn() const {return PromotionColumn;}
PieceColour Board::GetTurn() const {return turn;}
int Board::GetHalfmoveClock() const{return HalfmoveClock;}

bool Board::IsFiftyMoveDraw() const {
    return HalfmoveClock >= 100;
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

void Board::Promote(PieceType type) {
    if (!PromotionPending) {
        return;
    }

    squares[PromotionRow][PromotionColumn] = Piece(type, PromotionColour);
    PromotionPending = false;
    PromotionRow = -1;
    PromotionColumn = -1;

    if (turn == PieceColour::White) {
        turn = (turn == PieceColour::White) ? PieceColour::Black : PieceColour::White;
    }
}

void Board::MovePiece(
    int fromRow, 
    int fromColumn, 
    int toRow, 
    int toColumn)
{
    const auto movingPiece = squares[fromRow][fromColumn];
    // Check if a rook is about to be captured (for castling rights)
    const auto capturedPiece = squares[toRow][toColumn];

    if (movingPiece->type == PieceType::Pawn || capturedPiece){HalfmoveClock = 0;} // 50 move rule
    else{++HalfmoveClock;}

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
        const bool movesDiagonally = std::abs(fromColumn - toColumn) == 1;

        const bool destinationEmpty = !squares[toRow][toColumn];

        const bool targetMatches = enPassantTarget && toRow == enPassantTarget->first && toColumn == enPassantTarget->second;

        isEnPassant = movesDiagonally && destinationEmpty && targetMatches;
    }

    makeMove(fromRow, fromColumn, toRow, toColumn);

    // Remove the captured pawn during en passant
    if (isEnPassant)
    {
        squares[fromRow][toColumn].reset();
    }

    // Promotion
    if (movingPiece && movingPiece->type == PieceType::Pawn && (toRow == 0 || toRow == 7))
    {
        PromotionPending = true;
        PromotionColour = movingPiece->colour;
        PromotionRow = toRow;
        PromotionColumn = toColumn;
    }

    // Castling
    const auto& movedPiece = squares[toRow][toColumn];

    if (movedPiece && movedPiece->type == PieceType::King && std::abs(toColumn - fromColumn) == 2)
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
    if (movingPiece && movingPiece->type == PieceType::Pawn && std::abs(toRow - fromRow) == 2)
    {
        enPassantTarget = {(fromRow + toRow) / 2, fromColumn};
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
    if (!PromotionPending) {
        turn = (turn == PieceColour::White) ? PieceColour::Black : PieceColour::White;
    }
}

MoveState Board::MakeMove(const Move& move)
{
    // save current state
    MoveState state;
    state.capturedPiece = squares[move.toRow][move.toColumn];
    state.enPassantTarget = enPassantTarget;
    state.turn = turn;
    state.halfmoveClock = HalfmoveClock;

    state.whiteKingMoved = WhiteKingMoved;
    state.blackKingMoved = BlackKingMoved;

    state.whiteKingsideRookMoved = WhiteKingsideRookMoved;
    state.whiteQueensideRookMoved = WhiteQueensideRookMoved;
    state.blackKingsideRookMoved = BlackKingsideRookMoved;
    state.blackQueensideRookMoved = BlackQueensideRookMoved;

    const Piece& movingPiece = *squares[move.fromRow][move.fromColumn];


    const bool isEnPassant = movingPiece.type == PieceType::Pawn && move.fromColumn != move.toColumn && !squares[move.toRow][move.toColumn];

    if (isEnPassant){
        state.wasEnPassant = true;
        state.capturedPiece = squares[move.fromRow][move.toColumn];
        squares[move.fromRow][move.toColumn].reset();
    }


    if (movingPiece.type == PieceType::King){
        if (movingPiece.colour == PieceColour::White){
            WhiteKingMoved = true;
        }
        else{
            BlackKingMoved = true;
     }
     }
  
    if (movingPiece.type == PieceType::Rook){
        if (movingPiece.colour == PieceColour::White){
            if (move.fromRow == 7 && move.fromColumn == 0){
                WhiteQueensideRookMoved = true;
            }
            else if (move.fromRow == 7 && move.fromColumn == 7){
                WhiteKingsideRookMoved = true;
            }
        }
        else{
            if (move.fromRow == 0 && move.fromColumn == 0){
                BlackQueensideRookMoved = true;
            }
            else if (move.fromRow == 0 && move.fromColumn == 7){
                BlackKingsideRookMoved = true;
            }
        }
    }

    if (state.capturedPiece &&
        state.capturedPiece->type == PieceType::Rook){
        if (move.toRow == 7 && move.toColumn == 0){
            WhiteQueensideRookMoved = true;
        }
        else if (move.toRow == 7 && move.toColumn == 7){
            WhiteKingsideRookMoved = true;
        }
        else if (move.toRow == 0 && move.toColumn == 0){
            BlackQueensideRookMoved = true;
        }
        else if (move.toRow == 0 && move.toColumn == 7){
            BlackKingsideRookMoved = true;
        }
    }

    squares[move.toRow][move.toColumn] = squares[move.fromRow][move.fromColumn];
    squares[move.fromRow][move.fromColumn].reset();

    const bool isCastling = movingPiece.type == PieceType::King && std::abs(move.toColumn - move.fromColumn) == 2;

    if (isCastling){               
        state.wasCastling = true;
        const int row = move.fromRow;

        if (move.toColumn > move.fromColumn){
            // Kingside: h-file rook → f-file
            squares[row][5] = squares[row][7];
            squares[row][7].reset();
        }
        else{
            // Queenside: a-file rook → d-file
            squares[row][3] = squares[row][0];
            squares[row][0].reset();
        }
    }

    if (move.promotionPiece){
        squares[move.toRow][move.toColumn]->type = *move.promotionPiece;
    }
    enPassantTarget.reset();

    if (movingPiece.type == PieceType::Pawn && std::abs(move.toRow - move.fromRow) == 2)
    {
        const int middleRow = (move.fromRow + move.toRow) / 2;

        enPassantTarget = std::make_pair(middleRow, move.fromColumn);
    }

    if (movingPiece.type == PieceType::Pawn || state.capturedPiece)
    {
        HalfmoveClock = 0;
    }
    else
    {
        ++HalfmoveClock;
    }

    turn = (turn == PieceColour::White) ? PieceColour::Black : PieceColour::White;

    return state;
}

void Board::UnMakeMove(const Move& move, const MoveState& state)
{
    // Restore previous board state
    turn = state.turn;
    enPassantTarget = state.enPassantTarget;
    HalfmoveClock = state.halfmoveClock;

    WhiteKingMoved = state.whiteKingMoved;
    BlackKingMoved = state.blackKingMoved;

    WhiteKingsideRookMoved = state.whiteKingsideRookMoved;
    WhiteQueensideRookMoved = state.whiteQueensideRookMoved;
    BlackKingsideRookMoved = state.blackKingsideRookMoved;
    BlackQueensideRookMoved = state.blackQueensideRookMoved;


    // Undo castling
    if (state.wasCastling)
    {
        const int row = move.fromRow;

        if (move.toColumn > move.fromColumn)
        {
            // Kingside: f-file rook → h-file
            squares[row][7] = squares[row][5];
            squares[row][5].reset();
        }
        else
        {
            // Queenside: d-file rook → a-file
            squares[row][0] = squares[row][3];
            squares[row][3].reset();
        }
    }


    // Undo promotion
    if (move.promotionPiece)
    {
        squares[move.toRow][move.toColumn]->type = PieceType::Pawn;
    }


    // Move the piece back
    squares[move.fromRow][move.fromColumn] = squares[move.toRow][move.toColumn];

    squares[move.toRow][move.toColumn].reset();


    // Restore captured piece
    if (state.wasEnPassant)
    {
        squares[move.fromRow][move.toColumn] = state.capturedPiece;
    }
    else
    {
        squares[move.toRow][move.toColumn] = state.capturedPiece;
    }
}
