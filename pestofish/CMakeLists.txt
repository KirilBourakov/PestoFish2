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
        include/pestofish/Engine.hpp
        include/pestofish/State.hpp
        include/pestofish/ModuleOnly/Move.hpp
        include/pestofish/Evaluator.hpp
        include/pestofish/ZobristHash.hpp
        include/pestofish/Nnue.hpp

        include/pestofish/Board/BitBoard.hpp
        include/pestofish/Board/Board.hpp

        include/pestofish/ModuleOnly/Utils.hpp
        include/pestofish/ModuleOnly/Enums.hpp
        include/pestofish/ModuleOnly/PieceSqTables.hpp
        include/pestofish/ModuleOnly/TranspositionTable.hpp

        include/pestofish/Resources/buffer.hpp
        include/pestofish/Resources/bishop.h
        include/pestofish/Resources/rooks.h
        include/pestofish/Resources/model.h
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
