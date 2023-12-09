#include "thread_pool.h"
#include <unistd.h>


template <typename T>
ThreadPool<T>::ThreadPool(int number) : stop_(false)
{
	if(number <= 0 || number > MAX_THREADS) {
	    throw std::exception();
	}
	for(int i = 0; i < number; i++)
	{
		std::cout << "create " << i << " thread" << std::endl;
		work_threads_.emplace_back(worker,this);
	}
}

template <typename T>
inline ThreadPool<T>::~ThreadPool()
{
	std::unique_lock<std::mutex> lock(queue_mutex_);
	stop_ = true;

	condition_.notify_all(); //唤醒所有等待线程，类似的还有notify_one()
	for(auto &w : work_threads_)
	{
		w.join();
	}
}

template <typename T>
bool ThreadPool<T>::append(std::shared_ptr<T> request)
{
	queue_mutex_.lock(); 
	tasks_queue_.push(request); 
	queue_mutex_.unlock(); 
	condition_.notify_one(); // nitify thread pool about new task
	return true;
}

template <typename T>
void* ThreadPool<T>::worker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	pool->run(); // call run method
	return pool;
}

template <typename T>
void ThreadPool<T>::run()
{
	while(!stop_) {
		std::unique_lock<std::mutex> lk(this->queue_mutex_);
		/*unique_lock() 出作用域会自动解锁*/
		this->condition_.wait(lk,[this]{return !this->tasks_queue_.empty();}); //Lambda表达式
		//如果任务队列不为NULL，就停下来等待唤醒
		if(this->tasks_queue_.empty())
		{
			continue;
		}
		else
		{
			std::shared_ptr<T> request = tasks_queue_.front();
			tasks_queue_.pop();
			if(request) {
				request->process();
			}
		}
	}
}

class Task
{
public:
	Task(int i){ task_id_ = i;}
	void process()
	{
		std::cout << "-----run task" << task_id_ << ".........." << std::endl;
        sleep(2);
	}
private:
    int task_id_;

};


void taskProducer(ThreadPool<Task>& pool, int start, int end) {
    for (int i = start; i <= end; ++i) {
        std::shared_ptr<Task> t = std::make_shared<Task>(i);
        pool.append(t);
		std::cout << "put task " << i <<" to task queue" << std::endl;
    }
}

int main() {
    ThreadPool<Task> pool(6);

    std::thread thread1(taskProducer, std::ref(pool), 1, 10);
    std::thread thread2(taskProducer, std::ref(pool), 11, 20);

    thread1.join();
    thread2.join();

    // Continue with other operations or wait for the tasks to complete

    return 0;
}
