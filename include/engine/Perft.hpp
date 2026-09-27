#ifndef ENGINE_PERFT_HPP
#define ENGINE_PERFT_HPP

#include "core/Board.hpp"
#include <cstdint>

std::uint64_t Perft(Board& board, int depth); //node (position) count gets really large, so we use 64-bit unsigned 

#endif