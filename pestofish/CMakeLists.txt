cmake_minimum_required(VERSION 3.27)
project(cEngine)

set(CMAKE_CXX_STANDARD 20)

set(ENGINE_SOURCES
    src/Core/State.cpp
    src/Core/ZobristHash.cpp
    src/Core/Board/ArrayBoard.cpp
    src/Core/Board/BitBoard.cpp
    src/Eval/Evaluator.cpp
    src/Eval/Nnue.cpp
    src/Search/Engine.cpp
    src/Search/TranspositionTable.cpp
    src/Interface/UCI.cpp
)

set(ENGINE_HEADERS
    include/pestofish/Core/State.hpp
    include/pestofish/Core/Move.hpp
    include/pestofish/Core/Enums.hpp
    include/pestofish/Core/Utils.hpp
    include/pestofish/Core/parse.hpp
    include/pestofish/Core/PieceSqTables.hpp
    include/pestofish/Core/ZobristHash.hpp
    include/pestofish/Core/Board/Board.hpp
    include/pestofish/Core/Board/BitBoard.hpp
    include/pestofish/Core/Board/ArrayBoard.hpp
    
    include/pestofish/Eval/Evaluator.hpp
    include/pestofish/Eval/Nnue.hpp
    include/pestofish/Eval/Resources/buffer.hpp
    include/pestofish/Eval/Resources/bishop.h
    include/pestofish/Eval/Resources/rooks.h
    include/pestofish/Eval/Resources/model.h

    include/pestofish/Search/Engine.hpp
    include/pestofish/Search/TranspositionTable.hpp
    include/pestofish/Search/LazySmpThreads.hpp
    include/pestofish/Search/EngineOptions.hpp

    include/pestofish/Interface/UCI.hpp
)

# Create the library
add_library(pestofish_lib STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
target_include_directories(pestofish_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
    PUBLIC extern/cereal/include
)

# Main Executable
add_executable(pestofish2 src/main.cpp)
target_link_libraries(pestofish2 PRIVATE pestofish_lib)

# Benchmarks
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable benchmark's tests")
add_subdirectory(extern/benchmark)
add_executable(bench
    benchmark/main.cpp
    benchmark/preft.cpp
    benchmark/generateMoves.cpp
    benchmark/makeUndo.cpp
    benchmark/bitBoard.cpp
    test/Preft.hpp
)
target_include_directories(bench
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test
)
target_link_libraries(bench PRIVATE pestofish_lib benchmark::benchmark benchmark::benchmark_main)

# Tests
add_subdirectory(extern/googletest)
enable_testing()
add_executable(runTests
    test/TestMove.cpp
    test/TestPreft.cpp
    test/TestState.cpp
    test/TestEngine.cpp
    test/TestParse.cpp
    test/TestNnue.cpp
    test/Preft.hpp
)
target_include_directories(runTests
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test
)
target_link_libraries(runTests
    PRIVATE pestofish_lib
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

include(GoogleTest)
gtest_discover_tests(runTests)