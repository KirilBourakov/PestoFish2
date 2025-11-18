//
// Created by Kiril on 2025-11-17.
//

#pragma once
#include <algorithm>
#include <functional>
#include <mutex>
#include <queue>

class ThreadPool {
public:
    explicit ThreadPool(const int num_threads) {
        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::function<void()> task;

                    // get new tasks, or kill the thread
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
                    }

                    task();
                }
            });
        }
    }

    // Destructor to stop the thread pool
    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(taskMutex);
            stop = true;
        }

        condition.notify_all();
        for (auto& thread : threads) {
            thread.join();
        }
    }

    void enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(taskMutex);
            tasks.emplace(move(task));
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;

    std::queue<std::function<void()>> tasks;
    std::mutex taskMutex;

    std::condition_variable condition;

    bool stop = false;
};
