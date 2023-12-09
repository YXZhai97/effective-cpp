#ifndef _THREADPOOL_H
#define _THREADPOOL_H
#include <iostream>
#include <vector> 
#include <queue> 
#include <thread> 
#include <stdexcept> 
#include <condition_variable> 
#include <memory> 

const int MAX_THREADS = 1000; /


template <typename T>
class ThreadPool
{
public:
	ThreadPool(int number = 1);
	~ThreadPool();
	
	bool append(std::shared_ptr<T> request);

private:
	static void* worker(void* arg);
	void run();

private:
	std::vector<std::thread> work_threads_; 
	std::queue<std::shared_ptr<T>>tasks_queue_;
	std::mutex queue_mutex_;
	std::condition_variable condition_; 
	bool stop_;
};

#endif
