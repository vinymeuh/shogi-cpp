/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#ifndef SQUARE_H
#define SQUARE_H

#include <string>

namespace shogi {

enum class Color {
    black = 0, white = 1, none = 2
};

enum class Type {
    none = 0,
    pawn, lance, knight, silver, gold, bishop, rook, king,
};

Type pieceTypeFromUpperChar(char chr);

enum class Notation { USI, Hodges, Hosking };

class Piece {
public:
    Piece() : color{Color::none}, type{Type::none}, promoted{false} {};

    Piece(Color color, Type type, bool promoted=false);
    Piece(char chr, bool promoted=false);

    bool isValid();
    std::string toString() const;
    std::string toUpperString() const;

public:
    Color color;
    Type type;
    bool promoted;
};

extern const char* pieceNameChr;

class Square {
public:
    Square(): file{-1}, rank{-1}, piece{Piece{Color::none, Type::none}} {}
    Square(int file, int rank, Piece piece) : file{file}, rank{rank}, piece{piece} {};

    std::string coordinatesString(Notation format=Notation::USI) const;
public:
    int file;
    int rank;
    Piece piece;
};

} // namespace shogi
#endif // SQUARE_H
