#include <iostream>
#include <thread>
#include <mutex>

thread_local int tls_counter = 0;

void threadFunction(const std::string& thread_name) {
    for (int i=0; i < 10; ++i){
        tls_counter++;
        std::cout << "Thread " << thread_name << ": Counter = " << tls_counter << std::endl;
    }
}

int main() {
    // 创建多个线程
    std::thread t1(threadFunction,"th1");
    std::thread t2(threadFunction,"th2");
    std::thread t3(threadFunction,"th3");

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
