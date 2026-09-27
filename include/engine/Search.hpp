#ifndef ENGINE_SEARCH_HPP
#define ENGINE_SEARCH_HPP

#include "core/Board.hpp"
#include "core/Move.hpp"

class Search{
    public:
        static Move FindBestMove(Board& board, int depth);
    private:
        static int SearchPosition(Board& board, int depth);
};

#endif