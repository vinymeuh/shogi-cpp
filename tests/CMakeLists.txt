project(shogiTests LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
    SquareTest.cpp
    BoardTest.cpp
    MoveTest.cpp
    ShogiBoardTest.cpp
    MiniShogiBoardTest.cpp
)

include_directories(../src)

target_link_libraries(${PROJECT_NAME} PRIVATE shogi)
