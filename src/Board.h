/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#ifndef BOARD_H
#define BOARD_H

#include <string>
#include <sstream>
#include <map>
#include <vector>

#include "Square.h"
#include "Move.h"

namespace shogi {

class Board {
public:
    Board(unsigned int files, unsigned int ranks);

    void reset();

    unsigned int squares() const { return m_squares; }

    // Index <-> (File, Rank) conversion functions
    // Notes:
    //  - index is between [0, squares[ whereas file and rank are respectively between [1, files] and [1, ranks]
    //  - index origin (0) is upper left square and progress is from left to right
    //  - (file, rank) origin (1,1) is upper right square and progress is from right to left
    int toIndex(int file, int rank) const { return (rank-1)*m_files + (m_files - file); }
    int toFile(int index) const { return m_files-index%m_files; }
    int toRank(int index) const { return 1+(int)index/m_files; }

    Square& operator [](int idx);
    Square operator [](int idx) const;

    // Returns the color of the current side to move.
    Color sideToMove() const { return m_sidetomove; }
    unsigned int moveCount() const { return m_movecount; }

    void setSFEN(const std::string sfen);

    // Returns current state of the board as a SFEN string.
    std::string SFEN() const;

    std::string startingSFEN() const { return m_starting_sfen; }

    bool move(int from, int to, bool promote=false);
    bool drop(Color color, Type type, int to);

    bool moveUSI(const std::string &movestr);

    // moves log
    int moves() const { return m_moves.size(); }
    std::string moveNumber(int n, Notation format=Notation::USI) const { return m_moves[n].toString(format); }

    // hands
    int hand(Color color, Type type) const { return m_hand.at(color).at(type); }

public:
    virtual bool moveCanPromote(int from, int to) = 0;
    virtual bool moveMustPromote(int from, int to) = 0;

protected:
    // board characteriscs
    unsigned int m_files;   // columns
    unsigned int m_ranks;   // lines
    unsigned int m_squares; // lines x columns

    // board representation as an array of m_squares elements (square centric)
    std::vector<Square> m_board;

    // hands
    std::map<Color, std::map<Type, int>> m_hand;

    // moves log
    std::string m_starting_sfen;
    std::vector<Move> m_moves;

    // miscellaneous variables to manage state of the game
    Color m_sidetomove;
    unsigned int m_movecount;
};

} // namespace shogi
#endif // BOARD_H
