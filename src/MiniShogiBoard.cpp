/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include "MiniShogiBoard.h"

using namespace shogi;

namespace shogi {

bool MiniShogiBoard::moveCanPromote(int from, int to) {
    if (from < 0 || from >= m_squares || to < 0 || to >= m_squares) return false;

    Square square = m_board[from];
    if (square.piece.promoted == true) return false;

    switch (square.piece.color) {
    case Color::black:
        if (square.piece.type == Type::gold || square.piece.type == Type::king) return false;
        if (from <=4 || to <= 4) return true;
        return false;
    case Color::white:
        if (square.piece.type == Type::gold || square.piece.type == Type::king) return false;
        if (from >=20 || to >= 20) return true;
        return false;
    default:
        return false;
    }
}

bool MiniShogiBoard::moveMustPromote(int from, int to) {
    if (from >= m_squares || to >= m_squares) return false;
    Square square = m_board[from];
    return (square.piece.type == Type::pawn && moveCanPromote(from, to));
}

} // namespace shogi
