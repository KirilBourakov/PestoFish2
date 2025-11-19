//
// Created by Kiril on 2025-11-17.
//

#pragma once
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>

#include "EngineOptions.hpp"
#include "State.hpp"

using RootFunc = std::function<Move(State&, const std::vector<Move>&, SearchLimits, OrderingInfo&, RngInfo&, int&)>;

class LazySmpThreads {
public:
    explicit LazySmpThreads(const int num_threads, const int seed, const OrderingInfo& ordering)
        : nthreads(num_threads) {
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, 99999);

        for (int i = 0; i < num_threads; i++) {
            orderInfo.push_back(ordering);
            rngInfo.push_back(RngInfo::fromSeed(dist(rng)));
        }

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([this, i] {
                State threadCopy = state;
                while (true) {
                    RootFunc task;

                    {
                        std::unique_lock<std::mutex> lock(taskMutex);

                        condition.wait(lock, [this] {
                            return !tasks.empty() || stop;
                        });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = move(tasks.front());
                        tasks.pop();
                        threadCopy = state;
                    }

                    int out;
                    if (moves == nullptr) {
                        throw std::invalid_argument("Running task on nullptr");
                    }
                    task(threadCopy, *moves, limits, orderInfo.at(i), rngInfo.at(i), out);
                }
            });
        }
    }

    void sync(const State& currState, std::vector<Move>* currMoves) {
        state = currState.makeThreadCopy();
        moves = currMoves;
    }

    void enqueue(RootFunc task, const SearchLimits& limitsIn) {
        limits = limitsIn;
        for (int i = 0; i < nthreads; ++i) {
            {
                std::unique_lock<std::mutex> lock(taskMutex);
                tasks.emplace(i == nthreads - 1 ? move(task) : task);
            }
            condition.notify_one();
        }
    }

    void clearQueue() {
        std::unique_lock<std::mutex> lock(taskMutex);
        std::queue<RootFunc> empty;
        tasks.swap(empty);
    }

    ~LazySmpThreads() {
        {
            std::unique_lock<std::mutex> lock(taskMutex);
            stop = true;
        }

        condition.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

    const int nthreads;

private:
    std::vector<std::thread> threads;

    std::queue<RootFunc> tasks;
    std::vector<OrderingInfo> orderInfo;
    std::vector<RngInfo> rngInfo;
    SearchLimits limits{};

    State state;
    std::vector<Move>* moves = nullptr;

    std::mutex taskMutex;
    std::condition_variable condition;
    bool stop = false;
};
