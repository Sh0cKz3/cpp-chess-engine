#include "engine/ChessRules.hpp" //access to generateLegalMoves and MakeMove/UnMakeMove
#include "engine/Perft.hpp"

std::uint64_t Perft(Board& board, int depth) {
    if (depth == 0){
        return 1; //base case
    }

    const std::vector<Move> moves = ChessRules::GenerateLegalMoves(board);

    std::uint64_t nodes = 0;

    for (const Move& move : moves) {
        MoveState state = board.MakeMove(move);
        nodes += Perft(board, depth - 1);
        board.UnMakeMove(move, state);
    }
    
    return nodes;
}