/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#ifndef MINISHOGIBOARD_H
#define MINISHOGIBOARD_H

#include <bitset>
#include "Board.h"

namespace shogi {

/*
 * Board mapping
 * +----+----+----+----+----+
 * |  0 |  1 |  2 |  3 |  4 |
 * +----+----+----+----+----+
 * |  5 |  6 |  7 |  8 |  9 |
 * +----+----+----+----+----+
 * | 10 | 11 | 12 | 13 | 14 |
 * +----+----+----+----+----+
 * | 15 | 16 | 17 | 18 | 19 |
 * +----+----+----+----+----+
 * | 20 | 21 | 22 | 23 | 24 |
 * +----+----+----+----+----+
 *
 */

using bitboard25 = std::bitset<25>;

class MiniShogiBoard final : public Board
{
public:
   MiniShogiBoard() : Board(5, 5) {
        m_hand[Color::black] = {
            {Type::pawn, 0}, {Type::silver, 0},
            {Type::gold, 0}, {Type::bishop, 0}, {Type::rook, 0},
        };
        m_hand[Color::white] = {
            {Type::pawn, 0}, {Type::silver, 0},
            {Type::gold, 0}, {Type::bishop, 0}, {Type::rook, 0},
        };
    }

public:
    virtual bool moveCanPromote(int from, int to) override;
    virtual bool moveMustPromote(int from, int to) override;
};
} // namespace shogi
#endif // MINISHOGIBOARD_H
