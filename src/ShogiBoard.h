/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#ifndef SHOGIBOARD_H
#define SHOGIBOARD_H

#include "Board.h"

namespace shogi {

    namespace sfen {
        extern const std::string shogi_even;
        extern const std::string shogi_handicap_lance;
        extern const std::string shogi_handicap_bishop;
        extern const std::string shogi_handicap_rook;
        extern const std::string shogi_handicap_rook_lance;
        extern const std::string shogi_handicap_2pieces;
        extern const std::string shogi_handicap_4pieces;
        extern const std::string shogi_handicap_6pieces;
        extern const std::string shogi_handicap_8pieces;
        extern const std::string shogi_handicap_10pieces;
    }

class ShogiBoard final : public Board {
public:
    ShogiBoard() : Board(9, 9) {
        m_hand[Color::black] = {
            {Type::pawn, 0}, {Type::lance, 0}, {Type::knight, 0},
            {Type::silver, 0}, {Type::gold, 0}, {Type::bishop, 0}, {Type::rook, 0},
        };
        m_hand[Color::white] = {
            {Type::pawn, 0}, {Type::lance, 0}, {Type::knight, 0},
            {Type::silver, 0}, {Type::gold, 0}, {Type::bishop, 0}, {Type::rook, 0},
        };
    }

public:
    virtual bool moveCanPromote(int from, int to) override;
    virtual bool moveMustPromote(int from, int to) override;
};

} // namespace shogi
#endif // SHOGIBOARD_H
