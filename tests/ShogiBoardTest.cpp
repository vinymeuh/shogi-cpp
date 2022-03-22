#include <array>
#include "catch.hpp"
#include "ShogiBoard.h"

using namespace shogi;

// clazy:excludeall=non-pod-global-static

TEST_CASE("ShogiBoard: SFEN initializer") {
    ShogiBoard board;

    SECTION("SFEN even string") {
        std::string sfen = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";
        REQUIRE_NOTHROW(board.setSFEN(sfen));
        REQUIRE(board.sideToMove() == Color::black);
        REQUIRE(board.moveCount() == 1);
        REQUIRE(board.SFEN() == sfen);

        // hands
        std::array colors{Color::black, Color::white};
        std::array pieces{Type::pawn, Type::lance, Type::knight, Type::silver, Type::gold, Type::bishop, Type::rook};
        for (const auto& color : colors) {
            for (const auto& piece : pieces) {
                REQUIRE(board.hand(color, piece) == 0);
            }
        }
    }

    SECTION("Board from the 3rd game of the 19th Ryu-O match between Sato and Watanabe") {
        std::string sfen = "8l/1l+R2P3/p2pBG1pp/kps1p4/Nn1P2G2/P1P1P2PP/1PS6/1KSG3+r1/LN2+p3L w Sbgn3p 124";
        REQUIRE_NOTHROW(board.setSFEN(sfen));
        REQUIRE(board.sideToMove() == Color::white);
        REQUIRE(board.moveCount() == 124);
        REQUIRE(board.SFEN() == sfen);

        // hands
        REQUIRE(board.hand(Color::black, Type::silver) == 1);
        REQUIRE(board.hand(Color::white, Type::bishop) == 1);
        REQUIRE(board.hand(Color::white, Type::gold) == 1);
        REQUIRE(board.hand(Color::white, Type::knight) == 1);
        REQUIRE(board.hand(Color::white, Type::pawn) == 3);

        std::array black_pieces{Type::pawn, Type::lance, Type::knight, Type::gold, Type::bishop, Type::rook};
        for (const auto& piece : black_pieces) {
            REQUIRE(board.hand(Color::black, piece) == 0);
        }

        std::array white_pieces{Type::lance, Type::rook};
        for (const auto& piece : white_pieces) {
            REQUIRE(board.hand(Color::white, piece) == 0);
        }
    }
}

TEST_CASE("ShogiBoard: moveCanPromote & moveMustPromote") {
    ShogiBoard board;

    SECTION("King & Gold") {
        std::array pieces{
            Piece{Color::black, Type::king},
            Piece{Color::white, Type::king},
            Piece{Color::black, Type::gold},
            Piece{Color::white, Type::gold}
        };

        for (const auto& piece : pieces) {
            for (unsigned int from=0; from<board.squares(); from++) {
                board[from] = Square(board.toFile(from), board.toRank(from), piece);
                for (unsigned int to=0; to<board.squares(); to++) {
                    REQUIRE(board.moveCanPromote(from, to) == false);
                    REQUIRE(board.moveMustPromote(from, to) == false);
                }
            }
        }
    }

    SECTION("Silver, Bishop & Rook") {
        std::array pieces{
            Piece{Color::black, Type::silver},
            Piece{Color::white, Type::silver},
            Piece{Color::black, Type::bishop},
            Piece{Color::white, Type::bishop},
            Piece{Color::black, Type::rook},
            Piece{Color::white, Type::rook}
        };

        for (const auto& piece : pieces) {
            for (unsigned int from=0; from<board.squares(); from++) {
                // Black Silver
                board[from] = Square(board.toFile(from), board.toRank(from), piece);
                for (unsigned int to=0; to<board.squares(); to++) {
                    if ( (piece.color == Color::black && (from <=18 || to <=18)) ||
                         (piece.color == Color::white && (from >=54 || to >=54)) ) {
                        REQUIRE(board.moveCanPromote(from, to) == true);
                    }
                    else {
                        REQUIRE(board.moveCanPromote(from, to) == false);
                    }
                    REQUIRE(board.moveMustPromote(from, to) == false);
                }
            }
        }
    }

    SECTION("Knight") {
        std::array pieces{
            Piece{Color::black, Type::knight},
            Piece{Color::white, Type::knight}
        };

        for (const auto& piece : pieces) {
            for (unsigned int from=0; from<board.squares(); from++) {
                board[from] = Square(board.toFile(from), board.toRank(from), piece);
                for (unsigned int to=0; to<board.squares(); to++) {
                    if ( (piece.color == Color::black && (from <=18 || to <=18)) ||
                         (piece.color == Color::white && (from >=54 || to >=54)) ) {
                        REQUIRE(board.moveCanPromote(from, to) == true);
                    }
                    else {
                        REQUIRE(board.moveCanPromote(from, to) == false);
                    }
                    if ( (piece.color == Color::black && to <=17) ||
                         (piece.color == Color::white && to >=63) ) {
                        REQUIRE(board.moveMustPromote(from, to) == true);
                    }
                    else {
                        REQUIRE(board.moveMustPromote(from, to) == false);
                    }
                }
            }
        }
    }

    SECTION("Lance & Pawn") {
        std::array pieces{
            Piece{Color::black, Type::lance},
            Piece{Color::white, Type::lance},
            Piece{Color::black, Type::pawn},
            Piece{Color::white, Type::pawn}
        };

        for (const auto& piece : pieces) {
            for (unsigned int from=0; from<board.squares(); from++) {
                board[from] = Square(board.toFile(from), board.toRank(from), piece);
                for (unsigned int to=0; to<board.squares(); to++) {
                    if ( (piece.color == Color::black && (from <=18 || to <=18)) ||
                         (piece.color == Color::white && (from >=54 || to >=54)) ) {
                        REQUIRE(board.moveCanPromote(from, to) == true);
                    }
                    else {
                        REQUIRE(board.moveCanPromote(from, to) == false);
                    }
                    if ( (piece.color == Color::black && to <=8) ||
                         (piece.color == Color::white && to >=72) ) {
                        REQUIRE(board.moveMustPromote(from, to) == true);
                    }
                    else {
                        REQUIRE(board.moveMustPromote(from, to) == false);
                    }
                }
            }
        }
    }
}
