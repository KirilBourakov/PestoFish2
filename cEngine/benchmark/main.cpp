//
// Created by Kiril on 11/30/2025.
//

#include <benchmark/benchmark.h>

extern void registerPreftBenchmark();

int main(int argc, char** argv) {
    registerPreftBenchmark();

    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    return 0;
}