#include "catch.hpp"
#include "Square.h"

using namespace shogi;

// clazy:excludeall=non-pod-global-static

TEST_CASE("Square: Color is an Enum ordered from black to none")
{
    REQUIRE(static_cast<int>(Color::black) == 0);
    REQUIRE(static_cast<int>(Color::white) == 1);
    REQUIRE(static_cast<int>(Color::none) == 2);
}

TEST_CASE("Square: Type is an Enum ordered from empty to king")
{
    REQUIRE(static_cast<int>(Type::none) == 0);
    REQUIRE(static_cast<int>(Type::pawn) == 1);
    REQUIRE(static_cast<int>(Type::lance) == 2);
    REQUIRE(static_cast<int>(Type::knight) == 3);
    REQUIRE(static_cast<int>(Type::silver) == 4);
    REQUIRE(static_cast<int>(Type::gold) == 5);
    REQUIRE(static_cast<int>(Type::bishop) == 6);
    REQUIRE(static_cast<int>(Type::rook) == 7);
    REQUIRE(static_cast<int>(Type::king) == 8);
}

TEST_CASE("Square: Piece string representation")
{
    Piece piece;
    REQUIRE(piece.toString() == ".");

    piece = Piece(Color::black, Type::pawn); REQUIRE(piece.toString() == "P");
    piece = Piece(Color::black, Type::lance); REQUIRE(piece.toString() == "L");
    piece = Piece(Color::black, Type::knight); REQUIRE(piece.toString() == "N");
    piece = Piece(Color::black, Type::silver); REQUIRE(piece.toString() == "S");
    piece = Piece(Color::black, Type::gold); REQUIRE(piece.toString() == "G");
    piece = Piece(Color::black, Type::bishop); REQUIRE(piece.toString() == "B");
    piece = Piece(Color::black, Type::rook); REQUIRE(piece.toString() == "R");
    piece = Piece(Color::black, Type::king); REQUIRE(piece.toString() == "K");

    piece = Piece(Color::black, Type::pawn, true); REQUIRE(piece.toString() == "+P");
    piece = Piece(Color::black, Type::lance, true); REQUIRE(piece.toString() == "+L");
    piece = Piece(Color::black, Type::knight, true); REQUIRE(piece.toString() == "+N");
    piece = Piece(Color::black, Type::silver, true); REQUIRE(piece.toString() == "+S");
    piece = Piece(Color::black, Type::bishop, true); REQUIRE(piece.toString() == "+B");
    piece = Piece(Color::black, Type::rook, true); REQUIRE(piece.toString() == "+R");

    piece = Piece(Color::white, Type::pawn); REQUIRE(piece.toString() == "p");
    piece = Piece(Color::white, Type::lance); REQUIRE(piece.toString() == "l");
    piece = Piece(Color::white, Type::knight); REQUIRE(piece.toString() == "n");
    piece = Piece(Color::white, Type::silver); REQUIRE(piece.toString() == "s");
    piece = Piece(Color::white, Type::gold); REQUIRE(piece.toString() == "g");
    piece = Piece(Color::white, Type::bishop); REQUIRE(piece.toString() == "b");
    piece = Piece(Color::white, Type::rook); REQUIRE(piece.toString() == "r");
    piece = Piece(Color::white, Type::king); REQUIRE(piece.toString() == "k");

    piece = Piece(Color::white, Type::pawn, true); REQUIRE(piece.toString() == "+p");
    piece = Piece(Color::white, Type::lance, true); REQUIRE(piece.toString() == "+l");
    piece = Piece(Color::white, Type::knight, true); REQUIRE(piece.toString() == "+n");
    piece = Piece(Color::white, Type::silver, true); REQUIRE(piece.toString() == "+s");
    piece = Piece(Color::white, Type::bishop, true); REQUIRE(piece.toString() == "+b");
    piece = Piece(Color::white, Type::rook, true); REQUIRE(piece.toString() == "+r");
}

TEST_CASE("Square: Piece constructor with invalid inputs")
{
    REQUIRE_THROWS_AS(Piece(Color::none, Type::pawn, false), std::invalid_argument);
    REQUIRE_THROWS_AS(Piece(Color::black, Type::none, false), std::invalid_argument);
    REQUIRE_THROWS_AS(Piece(Color::white, Type::none, false), std::invalid_argument);
    REQUIRE_THROWS_AS(Piece(Color::black, Type::gold, true), std::invalid_argument);
    REQUIRE_THROWS_AS(Piece(Color::black, Type::king, true), std::invalid_argument);
}

TEST_CASE("Square: construct a Piece from a char")
{
    Piece piece;

    piece = Piece('P'); REQUIRE(piece.toString() == "P");
    piece = Piece('L'); REQUIRE(piece.toString() == "L");
    piece = Piece('N'); REQUIRE(piece.toString() == "N");
    piece = Piece('S'); REQUIRE(piece.toString() == "S");
    piece = Piece('G'); REQUIRE(piece.toString() == "G");
    piece = Piece('B'); REQUIRE(piece.toString() == "B");
    piece = Piece('R'); REQUIRE(piece.toString() == "R");
    piece = Piece('K'); REQUIRE(piece.toString() == "K");

    piece = Piece('P', true); REQUIRE(piece.toString() == "+P");
    piece = Piece('L', true); REQUIRE(piece.toString() == "+L");
    piece = Piece('N', true); REQUIRE(piece.toString() == "+N");
    piece = Piece('S', true); REQUIRE(piece.toString() == "+S");
    piece = Piece('B', true); REQUIRE(piece.toString() == "+B");
    piece = Piece('R', true); REQUIRE(piece.toString() == "+R");

    piece = Piece('p'); REQUIRE(piece.toString() == "p");
    piece = Piece('l'); REQUIRE(piece.toString() == "l");
    piece = Piece('n'); REQUIRE(piece.toString() == "n");
    piece = Piece('s'); REQUIRE(piece.toString() == "s");
    piece = Piece('g'); REQUIRE(piece.toString() == "g");
    piece = Piece('b'); REQUIRE(piece.toString() == "b");
    piece = Piece('r'); REQUIRE(piece.toString() == "r");
    piece = Piece('k'); REQUIRE(piece.toString() == "k");

    piece = Piece('p', true); REQUIRE(piece.toString() == "+p");
    piece = Piece('l', true); REQUIRE(piece.toString() == "+l");
    piece = Piece('n', true); REQUIRE(piece.toString() == "+n");
    piece = Piece('s', true); REQUIRE(piece.toString() == "+s");
    piece = Piece('b', true); REQUIRE(piece.toString() == "+b");
    piece = Piece('r', true); REQUIRE(piece.toString() == "+r");

    REQUIRE_THROWS_AS(Piece(' '), std::invalid_argument);
    REQUIRE_THROWS_AS(Piece('x'), std::invalid_argument);
}
