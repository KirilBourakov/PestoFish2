cmake_minimum_required(VERSION 3.27)
project(cEngine)

set(CMAKE_CXX_STANDARD 20)

include_directories(extern/cereal)

set(ENGINE_SOURCES
        src/Engine.cpp
        src/State.cpp
        src/Evaluator.cpp
        src/ZobristHash.cpp
        src/Nnue.cpp

        src/Board/ArrayBoard.cpp
        src/Board/BitBoard.cpp

)

set(ENGINE_MODULES
        include/Engine.hpp
        include/State.hpp
        include/ModuleOnly/Move.hpp
        include/Evaluator.hpp
        include/ZobristHash.hpp
        include/Nnue.hpp

        include/Board/BitBoard.hpp
        include/Board/Board.hpp

        include/ModuleOnly/Utils.hpp
        include/ModuleOnly/Enums.hpp
        include/ModuleOnly/PieceSqTables.hpp
        include/ModuleOnly/TranspositionTable.hpp
)

# Build UCI exposure
add_executable(pestofish2
        src/uci.cpp
        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
)
target_include_directories(pestofish2
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE extern/cereal/include/
)

# Build benchmarks
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable benchmark's tests")
add_subdirectory(extern/benchmark)
add_executable(bench
        benchmark/main.cpp
        benchmark/preft.cpp
        benchmark/generateMoves.cpp
        benchmark/makeUndo.cpp
        benchmark/bitBoard.cpp

        test/Preft.hpp

        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
)
target_include_directories(bench
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/
        PRIVATE extern/cereal/include/
)
target_link_libraries(bench PRIVATE benchmark::benchmark benchmark::benchmark_main)

# Build Tests
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
        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
)

target_link_libraries(runTests
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
)

target_include_directories(runTests
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/
        PRIVATE extern/cereal/include/
)

include(GoogleTest)
gtest_discover_tests(runTests)
