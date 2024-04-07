#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool dataProcessed = false;

void firstThreadFunction() {
    
    {
        std::lock_guard<std::mutex> lock(mtx);
        dataProcessed = true;
        std::cout << "dataProcessed true" << std::endl;
    }
    cv.notify_all();
}

void secondThreadFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout<< "Second thread get mtx" << std::endl;
        cv.wait(lock, []{ return dataProcessed; });
        std::cout<< "Second thread wait true" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    std::cout << "Second thread: Data processing completed." << std::endl;
}

void thirdThreadFunction() {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout<< "Third thread get mtx" << std::endl;
        cv.wait(lock, []{ return dataProcessed; });
        std::cout<< "Third thread wait true" << std::endl;

    }
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "Third thread: Data processing completed." << std::endl;
}

int main() {
    std::thread t1(firstThreadFunction);
    std::thread t2(secondThreadFunction);
    std::thread t3(thirdThreadFunction);

    t1.join();
    t2.join();
    t3.join();

    return 0;
}
