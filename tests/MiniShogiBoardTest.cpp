#include <array>
#include "catch.hpp"
#include "MiniShogiBoard.h"

using namespace shogi;

// clazy:excludeall=non-pod-global-static

TEST_CASE("MiniShogiBoard: SFEN initializer") {
    MiniShogiBoard board;

    SECTION("SFEN even string") {
        std::string sfen = "rbsgk/4p/5/P4/KGSBR b - 1";
        REQUIRE_NOTHROW(board.setSFEN(sfen));
        REQUIRE(board.sideToMove() == Color::black);
        REQUIRE(board.moveCount() == 1);
        REQUIRE(board.SFEN() == sfen);

        // hands
        std::array colors{Color::black, Color::white};
        std::array pieces{Type::pawn, Type::silver, Type::gold, Type::bishop, Type::rook};
        for (const auto& color : colors) {
            for (const auto& piece : pieces) {
                REQUIRE(board.hand(color, piece) == 0);
            }
        }
    }

    SECTION("SFEN with hands") {
        std::string sfen = "rbsgk/5/5/5/KGSBR b Pp 3";
        REQUIRE_NOTHROW(board.setSFEN(sfen));
        REQUIRE(board.sideToMove() == Color::black);
        REQUIRE(board.moveCount() == 3);
        REQUIRE(board.SFEN() == sfen);

        // hands
        REQUIRE(board.hand(Color::black, Type::pawn) == 1);
        REQUIRE(board.hand(Color::white, Type::pawn) == 1);
        std::array colors{Color::black, Color::white};
        std::array pieces{Type::silver, Type::gold, Type::bishop, Type::rook};
        for (const auto& color : colors) {
            for (const auto& piece : pieces) {
                REQUIRE(board.hand(color, piece) == 0);
            }
        }
    }
}

TEST_CASE("MiniShogiBoard: moveCanPromote & moveMustPromote") {
    MiniShogiBoard board;

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
                    if ( (piece.color == Color::black && (from <=4 || to <=4)) ||
                         (piece.color == Color::white && (from >=20 || to >=20)) ) {
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

    SECTION("Pawn") {
        std::array pieces{
            Piece{Color::black, Type::pawn},
            Piece{Color::white, Type::pawn}
        };

        for (const auto& piece : pieces) {
            for (unsigned int from=0; from<board.squares(); from++) {
                board[from] = Square(board.toFile(from), board.toRank(from), piece);
                for (unsigned int to=0; to<board.squares(); to++) {
                    if ( (piece.color == Color::black && (from <=4 || to <=4)) ||
                         (piece.color == Color::white && (from >=20 || to >=20)) ) {
                        REQUIRE(board.moveCanPromote(from, to) == true);
                        REQUIRE(board.moveMustPromote(from, to) == true);
                    }
                    else {
                        REQUIRE(board.moveCanPromote(from, to) == false);
                        REQUIRE(board.moveMustPromote(from, to) == false);
                    }
                }
            }
        }
    }

    SECTION("Invalid from/to") {
        REQUIRE(board.moveCanPromote(-1, 12) == false);
        REQUIRE(board.moveCanPromote(0, 25) == false);

        REQUIRE(board.moveMustPromote(-1, 12) == false);
        REQUIRE(board.moveMustPromote(0, 25) == false);
    }
}
