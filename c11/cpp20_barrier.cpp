#include <iostream>
#include <thread>
#include <vector>
#include <barrier>

// 定义一个回调函数，在所有线程到达屏障时执行
auto barrier_callback = []() {
    std::cout << "All threads have reached the barrier. Start executing callback..." <<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * 10));
    std::cout << "All threads have reached the barrier. End executing callback..." <<std::endl;
};

// 线程执行的任务
void task(int id, std::barrier<decltype(barrier_callback)>& sync_point) {
    std::cout << "Thread " << id << " is performing initial work"<< std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
    std::cout << "Thread " << id << " is waiting at the barrier" << std::endl;
    sync_point.arrive_and_wait();
    std::this_thread::sleep_for(std::chrono::milliseconds(100 * id));
    std::cout << "Thread " << id << " is performing final work" << std::endl;
}

int main() {
    // 定义一个 barrier屏障
    const int num_threads = 10;
    std::barrier sync_point(num_threads, barrier_callback);
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back([i, &sync_point]() {
            task(i, sync_point);
        });
    }
    for (auto& thread : threads) {
        thread.join();
    }
    return 0;
}
