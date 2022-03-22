/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#ifndef MOVE_H
#define MOVE_H

#include <optional>
#include <string>
#include "Square.h"

namespace shogi {

class Move {
public:
    Move(Square to, std::optional<Square> from=std::nullopt, bool promoted=false, bool promotable=false)
        : m_to{to}, m_from{from},
          m_promoted{promoted}, m_promotable{promotable} {};

    std::string toString(Notation format=Notation::USI) const;

private:
    Square m_to;
    std::optional<Square> m_from;   // no value for drop
    bool m_promoted;
    bool m_promotable;

};

} // namespace shogi
#endif // MOVE_H
