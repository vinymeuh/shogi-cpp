/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include "ShogiBoard.h"

using namespace shogi;

namespace shogi {

    namespace sfen {
        const std::string shogi_even = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
        const std::string shogi_handicap_lance = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/1NSGKGSNL w - 1";
        const std::string shogi_handicap_bishop = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/7R1/LNSGKGSNL w - 1";
        const std::string shogi_handicap_rook = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B7/LNSGKGSNL w - 1";
        const std::string shogi_handicap_rook_lance = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B7/1NSGKGSNL w - 1";
        const std::string shogi_handicap_2pieces = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/9/LNSGKGSNL w - 1";
        const std::string shogi_handicap_4pieces = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/9/1NSGKGSN1 w - 1";
        const std::string shogi_handicap_6pieces = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/9/2SGKGS2 w - 1";
        const std::string shogi_handicap_8pieces = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/9/3GKG3 w - 1";
        const std::string shogi_handicap_10pieces = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/9/4K4 w - 1";
    }

bool ShogiBoard::moveCanPromote(int from, int to) {
    Square square = m_board[from];
    if (square.piece.promoted == true) return false;

    switch (square.piece.color) {
    case Color::black:
        if (square.piece.type == Type::gold || square.piece.type == Type::king) return false;
        if (from <=18 || to <= 18) return true;
        return false;
    case Color::white:
        if (square.piece.type == Type::gold || square.piece.type == Type::king) return false;
        if (from >=54 || to >= 54) return true;
        return false;
    default:
        return false;
    }
}

bool ShogiBoard::moveMustPromote(int from, int to) {
    Square square = m_board[from];
    if (square.piece.promoted == true) return false;

    switch (square.piece.color) {
    case Color::black:
        if ((square.piece.type == Type::pawn || square.piece.type == Type::lance) && to <= 8) return true;
        if (square.piece.type == Type::knight && to <= 17) return true;
        return false;
    case Color::white:
        if ((square.piece.type == Type::pawn || square.piece.type == Type::lance) && to >= 72) return true;
        if (square.piece.type == Type::knight && to >= 63) return true;
        return false;
    default:
        return false;
    }
}

} // namespace shogi
