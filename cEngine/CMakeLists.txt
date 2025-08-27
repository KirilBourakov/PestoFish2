cmake_minimum_required(VERSION 3.27)
project(cEngine)

set(CMAKE_CXX_STANDARD 20)

set(pybind11_DIR "${CMAKE_SOURCE_DIR}/../.venv/Lib/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

# Build cEngine
pybind11_add_module(cEngine
        src/Engine.cpp
        src/Board.cpp
        src/Move.cpp
        src/Utils.cpp

        include/Engine.ixx
        include/Board.ixx
        include/Utils.ixx
        include/Types.ixx
        include/Move.ixx

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
        test/testBoards.cpp

        src/Board.cpp
        src/Move.cpp
        src/Utils.cpp

        include/Board.ixx
        include/Utils.ixx
        include/Types.ixx
        include/Move.ixx

)

target_link_libraries(runTests
        GTest::gtest_main
)

target_include_directories(runTests
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test/
)

include(GoogleTest)
gtest_discover_tests(runTests)
