/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include "Move.h"

using namespace shogi;

std::string Move::toString(Notation format) const {

    std::string s;

    switch(format) {
    case Notation::USI:
        if (m_from.has_value()) {
            s = m_from.value().coordinatesString() + m_to.coordinatesString();
            if (m_promoted == true) s+= "+";
        }
        else {
            s = m_to.piece.toUpperString() + "*" + m_to.coordinatesString();
        }
        break;
    case Notation::Hodges:
        if (m_from.has_value()) {
            s = m_from.value().piece.toUpperString(); // TODO: ambiguity resolution

            if (m_to.piece.type == Type::none) s += "-";
            else s += "x";

            s += m_to.coordinatesString();

            if (m_promoted == true) s+= "+";
            else if (m_promotable == true) s+= "=";
        }
        else {
            s = m_to.piece.toUpperString() + "*" + m_to.coordinatesString();
        }
        break;
    case Notation::Hosking:
        if (m_from.has_value()) {
            s = m_from.value().piece.toUpperString(); // TODO: ambiguity resolution

            if (m_to.piece.type != Type::none) s += "x";

            s += m_to.coordinatesString(Notation::Hosking);

            if (m_promoted == true) s+= "+";
            else if (m_promotable == true) s+= "=";
        }
        else {
            s = m_to.piece.toUpperString() + "'" + m_to.coordinatesString(Notation::Hosking);
        }
        break;
    }

    return s;
}
