/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include <sstream>
#include "Square.h"

namespace shogi {
    const char* pieceNameChr = ".PLNSGBRK";
}

using namespace shogi;

Type shogi::pieceTypeFromUpperChar(char chr) {
    switch(chr) {
    case 'P': return Type::pawn;
    case 'L': return Type::lance;
    case 'N': return Type::knight;
    case 'S': return Type::silver;
    case 'G': return Type::gold;
    case 'B': return Type::bishop;
    case 'R': return Type::rook;
    case 'K': return Type::king;
    default:
        throw std::invalid_argument("pieceTypeFromUpperChar called with invalid character");
    }
}

Piece::Piece(Color color, Type type, bool promoted)
    : color{color}, type{type}, promoted{promoted} {

    if (!isValid()) {
        throw std::invalid_argument("piece's constructor called with invalid arguments");
    }
}

Piece::Piece(char chr, bool promoted) : promoted{promoted} {
    color = Color::black;
    if (chr >= 'a' && chr < 'z') {
        color = Color::white;
        chr -= 'a' - 'A';
    }

    type = pieceTypeFromUpperChar(chr);

    if (!isValid()) {
        throw std::invalid_argument("piece's constructor called with invalid arguments");
    }
}

bool Piece::isValid() {
    if ((color == Color::none && type != Type::none) ||
        (color != Color::none && type == Type::none)) return false;
    if (promoted && (type == Type::gold || type == Type::king)) return false;
    return true;
}

std::string Piece::toString() const {
    std::ostringstream oss;
    if (promoted) oss << "+";
    char chr = pieceNameChr[static_cast<int>(type)];
    if (color == Color::white) chr += 'a' - 'A';
    oss << chr;
    return oss.str();
}

std::string Piece::toUpperString() const {
    std::ostringstream oss;
    if (promoted) oss << "+";
    char chr = pieceNameChr[static_cast<int>(type)];
    oss << chr;
    return oss.str();
}

std::string Square::coordinatesString(Notation format) const {
    switch(format) {
    case Notation::USI:
    case Notation::Hodges:
        return std::to_string(file) + (char)(char('a') + rank-1);
    case Notation::Hosking:
        return std::to_string(file) + std::to_string(rank);
    }
    return "??";
}
