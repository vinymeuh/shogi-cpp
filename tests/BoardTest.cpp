#include "catch.hpp"
#include "Board.h"

using namespace shogi;

// clazy:excludeall=non-pod-global-static

class FakeBoard final : public Board {
public:
    FakeBoard() : Board(2, 3) {
        m_hand[Color::black] = { {Type::pawn, 0} };
        m_hand[Color::white] = { {Type::pawn, 0} };
    };

    bool moveCanPromote(int from, int to) override { return true; }
    bool moveMustPromote(int from, int to) override { return true; }
};

TEST_CASE("Board: index <=> (file, rank) conversions") {
    FakeBoard board = FakeBoard();

    SECTION("(file, rank) => index") {
        REQUIRE(board.toIndex(1, 1) == 1);
        REQUIRE(board.toIndex(2, 1) == 0);
        REQUIRE(board.toIndex(1, 2) == 3);
        REQUIRE(board.toIndex(2, 2) == 2);
        REQUIRE(board.toIndex(1, 3) == 5);
        REQUIRE(board.toIndex(2, 3) == 4);
    }

    SECTION("index => file") {
        REQUIRE(board.toFile(0) == 2);
        REQUIRE(board.toFile(1) == 1);
        REQUIRE(board.toFile(2) == 2);
        REQUIRE(board.toFile(3) == 1);
        REQUIRE(board.toFile(4) == 2);
        REQUIRE(board.toFile(5) == 1);
    }

    SECTION("index => rank") {
        REQUIRE(board.toRank(0) == 1);
        REQUIRE(board.toRank(1) == 1);
        REQUIRE(board.toRank(2) == 2);
        REQUIRE(board.toRank(3) == 2);
        REQUIRE(board.toRank(4) == 3);
        REQUIRE(board.toRank(5) == 3);
    }
}

TEST_CASE("Board: basic board initialization") {
    FakeBoard board = FakeBoard();
    Square square;

    REQUIRE_THROWS_AS(board[-1], std::out_of_range);
    for (int i=0; i<board.squares(); i++) {
        // Check square access
        REQUIRE_NOTHROW(square = board[i]);
         // Check squares's coordinates are correct
        REQUIRE(square.file == board.toFile(i));
        REQUIRE(square.rank == board.toRank(i));
         // Check squares's are empty
        REQUIRE(square.piece.color == Color::none);
        REQUIRE(square.piece.type == Type::none);
        REQUIRE(square.piece.promoted == false);
    }
    REQUIRE_THROWS_AS(board[board.squares()+1], std::out_of_range);

    REQUIRE(board.hand(Color::black, Type::pawn) == 0);
    REQUIRE(board.hand(Color::white, Type::pawn) == 0);
}

TEST_CASE("Board: invalid moves & drops") {
    FakeBoard board;

    std::string sfen = "pk/2/KP b - 1";
    REQUIRE_NOTHROW(board.setSFEN(sfen));
    REQUIRE(board.SFEN() == sfen);

    // moves
    REQUIRE(board.move(5, 5) == false);   // same square
    REQUIRE(board.move(3, 5) == false);   // from empty square
    REQUIRE(board.move(-1, 5) == false);  // from out of board (low)
    REQUIRE(board.move(25, 5) == false);  // from out of board (high)
    REQUIRE(board.move(5, -1) == false);  // to out of board (low)
    REQUIRE(board.move(5, 25) == false);  // to out of board (high)
    REQUIRE(board.move(5, 4) == false);   // capture same color (black)
    REQUIRE(board.move(0, 1) == false);   // capture same color (white)
    REQUIRE(board.SFEN() == sfen);

    // drops
    REQUIRE(board.drop(Color::black, Type::king, 2) == false);    // drop king
    REQUIRE(board.drop(Color::black, Type::pawn, 1) == false);    // drop on non empty square
    REQUIRE(board.drop(Color::white, Type::bishop, 2) == false);  // drop piece not in hand
    REQUIRE(board.drop(Color::white, Type::pawn, -1) == false);   // drop out of board
    REQUIRE(board.drop(Color::white, Type::pawn, 25) == false);   // drop out of board
    REQUIRE(board.SFEN() == sfen);
}

TEST_CASE("Board: invalid moveUSI") {
    FakeBoard board;

    std::string sfen = "pk/2/KP b - 1";
    REQUIRE_NOTHROW(board.setSFEN(sfen));
    REQUIRE(board.SFEN() == sfen);

    // move - string length between 4 & 5
    REQUIRE_THROWS(board.moveUSI("1"));
    REQUIRE_THROWS(board.moveUSI("1a"));
    REQUIRE_THROWS(board.moveUSI("1a1"));
    REQUIRE_THROWS(board.moveUSI("1a1b++"));

    // move - first character is a number
    REQUIRE_THROWS(board.moveUSI("x11b"));
    REQUIRE_THROWS(board.moveUSI("*11b"));

    // move - second character is a lowercase letter
    REQUIRE_THROWS(board.moveUSI("111b"));
    REQUIRE_THROWS(board.moveUSI("1A1b"));

    // move - third character is a number
    REQUIRE_THROWS(board.moveUSI("1ayb"));
    REQUIRE_THROWS(board.moveUSI("1a!b"));

    // move - fourth character is a lowercase letter
    REQUIRE_THROWS(board.moveUSI("1a1B"));
    REQUIRE_THROWS(board.moveUSI("1a1!"));

    // move - fifth character can only be a '+'
    REQUIRE_THROWS(board.moveUSI("1a1b*"));

    // drop - first character is a valide upper piece letter
    REQUIRE_THROWS(board.moveUSI("Z*1a"));
    REQUIRE_THROWS(board.moveUSI("y*1a"));

    // drop - second character is always '*'
    REQUIRE_THROWS(board.moveUSI("P+1a"));

    // drop - third character is a number
    REQUIRE_THROWS(board.moveUSI("P*yb"));
    REQUIRE_THROWS(board.moveUSI("P*!b"));

    // drop - fourth character is a lowercase letter
    REQUIRE_THROWS(board.moveUSI("P*1B"));
    REQUIRE_THROWS(board.moveUSI("P*1!"));

    // drop - string length is always 4
    REQUIRE_THROWS(board.moveUSI("P*1a+"));
}


TEST_CASE("Board: move & drop") {
    FakeBoard board;

    std::string sfen = "pp/2/PP b - 1";
    REQUIRE_NOTHROW(board.setSFEN(sfen));
    REQUIRE(board.SFEN() == sfen);

    // simple move
    REQUIRE(board.move(4, 2) == true);
    REQUIRE(board.move(1, 3) == true);
    REQUIRE(board.SFEN() == "p1/Pp/1P b - 3");

    // capture
    REQUIRE(board.move(2, 0) == true);
    REQUIRE(board.move(3, 5) == true);
    REQUIRE(board.SFEN() == "P1/2/1p b Pp 5");
    REQUIRE(board.hand(Color::black, Type::pawn) == 1);
    REQUIRE(board.hand(Color::white, Type::pawn) == 1);

    // drop
    REQUIRE(board.drop(Color::black, Type::pawn, 1) == true);
    REQUIRE(board.drop(Color::white, Type::pawn, 4) == true);
    REQUIRE(board.SFEN() == "PP/2/pp b - 7");
    REQUIRE(board.hand(Color::black, Type::pawn) == 0);
    REQUIRE(board.hand(Color::white, Type::pawn) == 0);

    // promotion
    REQUIRE(board.move(0, 2, true) == true);
    REQUIRE(board.move(5, 3, true) == true);
    REQUIRE(board.SFEN() == "1P/+P+p/p1 b - 9");
}

TEST_CASE("Board: moveUSI") {
    FakeBoard board;

    std::string sfen = "pp/2/PP b - 1";
    REQUIRE_NOTHROW(board.setSFEN(sfen));
    REQUIRE(board.SFEN() == sfen);

    // simple move
    REQUIRE(board.moveUSI("2c2b") == true);
    REQUIRE(board.moveUSI("1a1b") == true);
    REQUIRE(board.SFEN() == "p1/Pp/1P b - 3");

    // capture
    REQUIRE(board.moveUSI("2b2a") == true);
    REQUIRE(board.moveUSI("1b1c") == true);
    REQUIRE(board.SFEN() == "P1/2/1p b Pp 5");
    REQUIRE(board.hand(Color::black, Type::pawn) == 1);
    REQUIRE(board.hand(Color::white, Type::pawn) == 1);

    // drop
    REQUIRE(board.moveUSI("P*1a") == true);
    REQUIRE(board.moveUSI("P*2c") == true);
    REQUIRE(board.SFEN() == "PP/2/pp b - 7");
    REQUIRE(board.hand(Color::black, Type::pawn) == 0);
    REQUIRE(board.hand(Color::white, Type::pawn) == 0);

    // promotion
    REQUIRE(board.moveUSI("2a2b+") == true);
    REQUIRE(board.moveUSI("1c1b+") == true);
    REQUIRE(board.SFEN() == "1P/+P+p/p1 b - 9");

    // moves log
    REQUIRE(board.moveCount() == 9);
    REQUIRE(board.moves() == 8);
    REQUIRE(board.moveNumber(0) == "2c2b");
    REQUIRE(board.moveNumber(1) == "1a1b");
    REQUIRE(board.moveNumber(2) == "2b2a");
    REQUIRE(board.moveNumber(3) == "1b1c");
    REQUIRE(board.moveNumber(4) == "P*1a");
    REQUIRE(board.moveNumber(5) == "P*2c");
    REQUIRE(board.moveNumber(6) == "2a2b+");
    REQUIRE(board.moveNumber(7) == "1c1b+");
}
