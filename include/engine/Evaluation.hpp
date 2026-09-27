#ifndef ENGINE_EVALUATION_HPP
#define ENGINE_EVALUATION_HPP

#include "core/Board.hpp"

class Evaluation {
    public:
        static int EvaluateBoard(const Board& board); //const, only returns a value
};

#endif 