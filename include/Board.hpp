#ifndef BOARD_HPP
#define BOARD_HPP

#include "Piece.hpp"

#include <optional>
#include <array>

class Board
{
private:
    std::array<std::array<std::optional<Piece>, 8>, 8> squares;

public:
    Board();

    const std::optional<Piece>& getPiece(int row, int column) const;
};

#endif