//
// Created by Kiril on 2025-11-17.
//

#pragma once
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>

#include "EngineOptions.hpp"
#include "../State.hpp"

using RootFunc = std::function<Move(State&, const std::vector<Move>&, SearchLimits, OrderingInfo&, RngInfo&, int&, Nnue&)>;

class LazySmpThreads {
public:
    explicit LazySmpThreads(const int num_threads, const int seed, const OrderingInfo& ordering)
        : nthreads(num_threads) {
        std::mt19937 rng(seed);
        std::uniform_int_distribution<int> dist(0, 99999);

        for (int i = 0; i < num_threads; i++) {
            orderInfo.push_back(ordering);
            rngInfo.push_back(RngInfo::fromSeed(dist(rng)));
            nnues.emplace_back();
        }

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([this, i] {
                State threadCopy;
                RootFunc task;
                std::vector<Move> movesCopy;
                SearchLimits limitsCopy;
                while (true) {
                    {
                        std::unique_lock<std::mutex> lock(taskMutex);

                        condition.wait(lock, [this] {
                            return !tasks.empty() || destructorStop;
                        });

                        if (destructorStop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop();
                        busy_count++;
                        threadCopy = state;
                        movesCopy = moves;
                        limitsCopy = limits;
                    }

                    int out;
                    task(threadCopy, movesCopy, limitsCopy, orderInfo.at(i), rngInfo.at(i), out, nnues.at(i));

                    {
                        std::lock_guard<std::mutex> lock(taskMutex);
                        this->busy_count--;
                        if(busy_count == 0 && tasks.empty()) {
                            cv_finished.notify_all();
                        }
                    }
                }
            });
        }
    }

    /**
     * Updates State and Moves used by Lazy SMP Threads
     */
    void sync(const State& currState, const std::vector<Move> &currMoves, const Nnue& nnue) {
        std::unique_lock<std::mutex> lock(taskMutex);
        state = currState.makeThreadCopy();
        moves = currMoves;
        for (const auto & i : nnues) {
            i.syncAccumulator(nnue);
        }
    }

    /**
     * Gives each thread a new search task
     */
    void enqueue(RootFunc task, const SearchLimits& limitsIn) {
        std::unique_lock<std::mutex> lock(taskMutex);
        limits = limitsIn;
        for (int i = 0; i < nthreads; ++i) {
            {
                tasks.emplace(i == nthreads - 1 ? std::move(task) : task);
            }
            condition.notify_one();
        }
        lock.unlock();
        condition.notify_all();
    }

    /**
     * Drops any tasks not in use
     */
    void clearQueue() {
        std::unique_lock<std::mutex> lock(taskMutex);
        std::queue<RootFunc> empty;
        tasks.swap(empty);
    }

    /**
     * Waits until all tasks are finished
     */
    void waitForIdle() {
        std::unique_lock<std::mutex> lock(taskMutex);
        cv_finished.wait(lock, [this]() {
            return this->tasks.empty() && (this->busy_count == 0);
        });
    }

    ~LazySmpThreads() {
        {
            std::unique_lock<std::mutex> lock(taskMutex);
            destructorStop = true;
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
    std::vector<Nnue> nnues;
    SearchLimits limits{};

    State state;
    std::vector<Move> moves;

    std::mutex taskMutex;
    std::condition_variable condition;
    bool destructorStop = false;

    int busy_count = 0;
    std::condition_variable cv_finished;
};
