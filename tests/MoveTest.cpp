#include <string>
#include <tuple>
#include "catch.hpp"
#include "Move.h"

using namespace shogi;

// clazy:excludeall=non-pod-global-static



TEST_CASE("Move - USI") {

    SECTION ("move") {
        auto data = GENERATE(table<Move, std::string>({
            // move to empty square
            { Move{Square{5, 3, Piece{}}, Square{5, 4, Piece{Color::black, Type::pawn}}}, "5d5c"},
            { Move{Square{1, 3, Piece{}}, Square{1, 2, Piece{Color::white, Type::pawn}}}, "1b1c"},
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::white, Type::silver, true}}}, "3e3a"},
            // move with capture
            { Move{Square{5, 3, Piece{Color::white, Type::pawn}}, Square{5, 4, Piece{Color::black, Type::pawn}}}, "5d5c"},
            // move with promotion
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::black, Type::silver}}, true}, "3e3a+"},
        }));

        REQUIRE(std::get<0>(data).toString() == std::get<1>(data));
    }


    SECTION("drop") {
        auto data = GENERATE(table<Move, std::string>({
            { Move{Square{3, 3, Piece{Color::black, Type::pawn}}}, "P*3c"},
            { Move{Square{4, 2, Piece{Color::white, Type::pawn}}}, "P*4b"},
        }));

        REQUIRE(std::get<0>(data).toString() == std::get<1>(data));
    }
}

TEST_CASE("Move - Hodges") {

    SECTION("move") {
        auto data = GENERATE(table<Move, std::string>({
            // move to empty square
            { Move{Square{5, 3, Piece{}}, Square{5, 4, Piece{Color::black, Type::pawn}}}, "P-5c"},
            { Move{Square{1, 3, Piece{}}, Square{1, 2, Piece{Color::white, Type::king}}}, "K-1c"},
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::white, Type::silver, true}}}, "+S-3a"},
            // move with capture
            { Move{Square{5, 3, Piece{Color::white, Type::pawn}}, Square{5, 4, Piece{Color::black, Type::pawn}}}, "Px5c"},
            // move with promotion
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::black, Type::silver}}, true}, "S-3a+"},
            { Move{Square{5, 3, Piece{Color::white, Type::pawn}}, Square{5, 4, Piece{Color::black, Type::pawn}}, true}, "Px5c+"},
            // move with promotion declined
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::black, Type::silver}}, false, true}, "S-3a="},
        }));

        REQUIRE(std::get<0>(data).toString(Notation::Hodges) == std::get<1>(data));
    }

    SECTION("drop") {
        auto data = GENERATE(table<Move, std::string>({
            { Move{Square{3, 3, Piece{Color::black, Type::pawn}}}, "P*3c"},
            { Move{Square{4, 2, Piece{Color::white, Type::pawn}}}, "P*4b"},
        }));

        REQUIRE(std::get<0>(data).toString(Notation::Hodges) == std::get<1>(data));
    }
}

TEST_CASE("Move - Hosking") {

    SECTION("move") {
        auto data = GENERATE(table<Move, std::string>({
            // move to empty square
            { Move{Square{5, 3, Piece{}}, Square{5, 4, Piece{Color::black, Type::pawn}}}, "P53"},
            { Move{Square{1, 3, Piece{}}, Square{1, 2, Piece{Color::white, Type::king}}}, "K13"},
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::white, Type::silver, true}}}, "+S31"},
            // move with capture
            { Move{Square{5, 3, Piece{Color::white, Type::pawn}}, Square{5, 4, Piece{Color::black, Type::pawn}}}, "Px53"},
            // move with promotion
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::black, Type::silver}}, true}, "S31+"},
            { Move{Square{5, 3, Piece{Color::white, Type::pawn}}, Square{5, 4, Piece{Color::black, Type::pawn}}, true}, "Px53+"},
            // move with promotion declined
            { Move{Square{3, 1, Piece{}}, Square{3, 5, Piece{Color::black, Type::silver}}, false, true}, "S31="},
        }));

        REQUIRE(std::get<0>(data).toString(Notation::Hosking) == std::get<1>(data));
    }

    SECTION("drop") {
        auto data = GENERATE(table<Move, std::string>({
            { Move{Square{3, 3, Piece{Color::black, Type::pawn}}}, "P'33"},
            { Move{Square{4, 2, Piece{Color::white, Type::pawn}}}, "P'42"},
        }));

        REQUIRE(std::get<0>(data).toString(Notation::Hosking) == std::get<1>(data));
    }
}
