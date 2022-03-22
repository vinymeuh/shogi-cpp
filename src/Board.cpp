/*
 * Copyright (c) VinyMeuh. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root for details.
 */

#include <sstream>
#include "Board.h"

using namespace shogi;

Board::Board(unsigned int files, unsigned int ranks)
    : m_ranks{ranks}, m_files{files}, m_squares{ranks*files} {
    for (int i=0; i<m_squares; i++) {
        m_board.push_back(Square{toFile(i), toRank(i), Piece{Color::none, Type::none}});
    }

    m_hand = { {Color::none, {}} };
};

void Board::reset() {
    for (auto &square : m_board) square.piece = Piece();
    for (auto& [key, value] : m_hand[Color::black]) value = 0;
    for (auto& [key, value] : m_hand[Color::white]) value = 0;
    m_moves.clear();

    m_starting_sfen = "";
    m_sidetomove = Color::black;
    m_movecount = 0;
}

Square& Board::operator [](int idx) {
    if (idx <0 || idx >= m_squares) throw std::out_of_range("invalid square index");
    return m_board[idx];
}

Square Board::operator [](int idx) const {
    if (idx <0 || idx >= m_squares) throw std::out_of_range("invalid square index");
    return m_board[idx];
}

void Board::setSFEN(const std::string sfen) {
    reset();
    m_starting_sfen = sfen;

    std::string token;

    // sfen consists of 3 to 4 fields white space separated
    std::vector<std::string> fields;
    std::istringstream iss(sfen);
    while (iss >> token) {
        fields.push_back(token);
    }
    if (auto s = fields.size(); s < 3 || s > 4) {
        throw std::invalid_argument("invalid sfen: must have 3 (optionally 4) fields space separated");
    }

    // fields[0] = Board state
    std::istringstream board_state(fields[0]);
    std::vector<std::string> ranks;

    while (std::getline(board_state, token, '/')) {
        ranks.push_back(token);
    }
    if (ranks.size() != m_ranks) {
        throw std::invalid_argument("invalid sfen: missing at least one rank in board state");
    }

    unsigned int file;
    bool promoted = false;
    for (unsigned int rank=0; rank<m_ranks; ++rank) {
        file = 0;   // TODO: check file < m_files
        for (const char& pieceChr : ranks[rank]) {
            if (isdigit(pieceChr)) {
                file += int(pieceChr) - '0';
            }
            else {
                if (pieceChr == '+') {
                    promoted = true;
                }
                else {
                    // Warning: (file, rank) here start to upper left square so we can't use m_board[toIndex(file, rank)]
                    m_board[rank*m_files+file].piece = Piece{pieceChr, promoted};
                    promoted = false;
                    file++;
                }
            }
        }
    }

    // fields[1] = Side to move
    if (fields[1] == "b") m_sidetomove = Color::black;
    else if (fields[1] == "w") m_sidetomove = Color::white;
    else throw std::invalid_argument("invalid sfen: side to move filed must be 'b' or 'w'");

    // fields[2] = Pieces in hand
    Piece piece;
    unsigned short piece_count = 1;
    for (const char& token : fields[2]) {
        if (token == '-' ) break;
        if (isdigit(token)) {
            piece_count = int(token) - '0';
            continue;
        }
        piece = Piece(token);
        m_hand[piece.color][piece.type] = piece_count;
        piece_count = 1;
    }

    // fields[3] = Move count (optional)
    if (fields.size() == 3) m_movecount = 1;
    else m_movecount = std::stoul(fields[3]);
}

std::string Board::SFEN() const {
    std::ostringstream sfen;

    // board state
    std::string cell;
    int ecc = 0; // empty cell counter
    for (int i=0; i<m_squares; ++i) {
        if (i%m_files == 0 && i != 0) {   // new line
            if (ecc > 0) {
                sfen << std::to_string(ecc);
                ecc = 0;
            }
            sfen << "/";
        }

        Square square = m_board[i];
        if (square.piece.type == Type::none) {
            ++ecc;
        }
        else {
            if (ecc > 0) {  // a piece after at least one empty cell
                sfen << std::to_string(ecc);
                ecc = 0;
            }
            sfen << square.piece.toString();
        }
    }
    if (ecc > 0) sfen << std::to_string(ecc);   // for when last line end with one or more empty square

    // side to move
    if (m_sidetomove == Color::black) sfen << " b ";
    else sfen << " w ";

    // hands
    int count;
    bool no_capture = true;
    for (Color color = Color::black; color < Color::none; color = Color(static_cast<int>(color)+1)) {
        auto hand = m_hand.find(color)->second;     // use find() because of const
        for (Type type = Type::rook; type > Type::none; type = Type(static_cast<int>(type)-1)) {
            count = hand[type];
            if (count == 0) continue;
            no_capture = false;
            if (count > 1) sfen << std::to_string(count);
            auto piece = Piece(color, type);
            sfen << piece.toString();
        }
    }
    if (no_capture) sfen << "-";

    // move count
    sfen << " " + std::to_string(m_movecount);

    return sfen.str();
};

bool Board::move(int from, int to, bool promote) {
    if (from < 0 || from >= m_squares || to < 0 || to >= m_squares) return false;
    Square &sfrom = m_board[from];
    Square &sto = m_board[to];

    // from empty square or destination occupied by one of own pieces
    if ( (sfrom.piece.type == Type::none) ||
         (sfrom.piece.color == sto.piece.color) ) return false;

    // capture ?
    if (sto.piece.type != Type::none) {
        m_hand[sfrom.piece.color][sto.piece.type] += 1;
    }

    // log move
    int promotable = moveCanPromote(from, to);
    Move m = Move(sto, sfrom, promote, promotable);
    m_moves.push_back(m);

    // move
    sto.piece.color = sfrom.piece.color;
    sto.piece.type = sfrom.piece.type;
    if (promote == true) sto.piece.promoted = true;

    sfrom.piece.color = Color::none;
    sfrom.piece.type = Type::none;
    sfrom.piece.promoted = false;

    // ready for next move
    m_sidetomove = (m_sidetomove == Color::black) ? Color::white : Color::black;
    m_movecount++;

    return true;
}

bool Board::drop(Color color, Type type, int to) {
    if (to < 0 || to >= m_squares || m_hand[color][type] <= 0) return false;
    Square &sto = m_board[to];

    // drop on non empty square
    if (sto.piece.type != Type::none) return false;

    // move
    sto.piece.color = color;
    sto.piece.type = type;
    sto.piece.promoted = false;
    m_hand[color][type] -= 1;

    // log move
    m_moves.push_back(sto);

    // ready for next move
    m_sidetomove = (m_sidetomove == Color::black) ? Color::white : Color::black;
    m_movecount++;

    return true;
}

bool Board::moveUSI(const std::string &movestr) {
    if (movestr.length() < 4) {
        throw std::invalid_argument("invalid usi move string");
    }

    if (isdigit(movestr[0])) {  // move
        if (!islower(movestr[1])) {
            throw std::invalid_argument("invalid usi move string");
        }
        int from = toIndex(movestr[0] - '0', 1 + movestr[1] - 'a');

        if (!isdigit(movestr[2]) || !islower(movestr[3])) {
            throw std::invalid_argument("invalid usi move string");
        }
        int to = toIndex(movestr[2] - '0', 1 + movestr[3] - 'a');

        if (movestr.length() == 4) {
            return move(from, to);
        }
        else if (movestr.length() == 5 && movestr[4] == '+') {
            return move(from, to, true);
        }
        else {
            throw std::invalid_argument("invalid usi move string");
        }
    }
    else {  // drop
        Type type = pieceTypeFromUpperChar(movestr[0]);
        if (movestr[1] != '*' ||!isdigit(movestr[2]) || !islower(movestr[3])) {
            throw std::invalid_argument("invalid usi move string");
        }

        int to = toIndex(movestr[2] - '0', 1 + movestr[3] - 'a');

        if (movestr.length() == 4) {
            return drop(m_sidetomove, type, to);
        }
        else {
            throw std::invalid_argument("invalid usi move string");
        }
    }
}
