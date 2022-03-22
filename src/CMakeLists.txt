set(HEADER_FILES
    shogi.h
    Board.h
    Square.h
    Move.h
    ShogiBoard.h
    MiniShogiBoard.h
)

set(SOURCE_FILES
    Board.cpp
    Square.cpp
    Move.cpp
    ShogiBoard.cpp
    MiniShogiBoard.cpp
)

add_library(shogi ${SOURCE_FILES} ${HEADER_FILES})
