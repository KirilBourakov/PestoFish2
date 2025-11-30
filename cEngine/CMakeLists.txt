cmake_minimum_required(VERSION 3.27)
project(cEngine)

set(CMAKE_CXX_STANDARD 20)

set(pybind11_DIR "${CMAKE_SOURCE_DIR}/../.venv/Lib/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

set(ENGINE_SOURCES
        src/Engine.cpp
        src/State.cpp
        src/NewBoard.cpp
        src/Evaluator.cpp
        src/ZobristHash.cpp
)

set(ENGINE_MODULES
        include/Engine.hpp
        include/State.hpp
        include/ModuleOnly/Move.hpp
        include/Evaluator.hpp
        include/ZobristHash.hpp

        include/ModuleOnly/CastleUtils.hpp
        include/ModuleOnly/Enums.hpp
        include/ModuleOnly/PieceSqTables.hpp
        include/ModuleOnly/TranspositionTable.hpp
)

# Build cEngine
pybind11_add_module(cEngine
        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
        pybind/bindings.cpp
)
target_include_directories(cEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# Build UCI exposure
add_executable(uci
        src/uci.cpp
        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
)
target_include_directories(uci
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# Build benchmarks
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "Disable benchmark's tests")
add_subdirectory(extern/benchmark)
add_executable(manual
        test/testBoard.hpp
        benchmark/preftBenchmark.cpp

        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
)
target_include_directories(manual
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/test/
)
target_link_libraries(manual PRIVATE benchmark::benchmark benchmark::benchmark_main)

# Build Tests
add_subdirectory(extern/googletest)
enable_testing()
add_executable(runTests
        test/TestMove.cpp
        test/TestPreft.cpp
        test/TestState.cpp
        test/TestEngine.cpp
        test/TestParse.cpp

        test/testBoard.hpp
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
)

include(GoogleTest)
gtest_discover_tests(runTests)
