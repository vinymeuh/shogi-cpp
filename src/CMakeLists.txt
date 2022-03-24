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

target_compile_options(shogi PUBLIC -Wall -pedantic)
target_link_libraries(shogi PUBLIC coverage_config)
