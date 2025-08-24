cmake_minimum_required(VERSION 3.27)
project(cEngine)

set(CMAKE_CXX_STANDARD 20)

set(pybind11_DIR "${CMAKE_SOURCE_DIR}/../.venv/Lib/site-packages/pybind11/share/cmake/pybind11")
find_package(pybind11 REQUIRED)

# Build cEngine
pybind11_add_module(cEngine
        src/Engine.cpp
        src/Board.cpp

        pybind/bindings.cpp
)
target_include_directories(cEngine
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# Build Tests
add_subdirectory(extern/googletest)
enable_testing()
add_executable(runTests
        test/RunTests.cpp
)

target_link_libraries(runTests
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(runTests)
