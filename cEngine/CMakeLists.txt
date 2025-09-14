cmake_minimum_required(VERSION 3.27)
project(cEngine)

set(CMAKE_CXX_STANDARD 20)

set(pybind11_DIR "${CMAKE_SOURCE_DIR}/../.venv/Lib/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

set(ENGINE_SOURCES
        src/Engine.cpp
        src/State.cpp
        src/Board.cpp
        src/Move.cpp
        src/Utils.cpp
        src/Evaluator.cpp
)

set(ENGINE_MODULES
        include/Engine.ixx
        include/State.ixx
        include/Board.ixx
        include/Utils.ixx
        include/Types.ixx
        include/Move.ixx
        include/Evaluator.ixx
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

# Build Tests
add_subdirectory(extern/googletest)
enable_testing()
add_executable(runTests
        test/TestMove.cpp
        test/TestPreft.cpp
        test/TestState.cpp

        test/testBoards.cpp
        test/testBoard.ixx
        ${ENGINE_SOURCES}
        ${ENGINE_MODULES}
)

target_link_libraries(runTests
        GTest::gtest_main
        GTest::gmock
        GTest::gmock_main
)

target_include_directories(runTests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/
)

include(GoogleTest)
gtest_discover_tests(runTests)
