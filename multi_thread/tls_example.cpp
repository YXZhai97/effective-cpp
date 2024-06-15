#include <iostream>
#include <pthread.h>

pthread_key_t tls_key;

void cleanup(void* value) {
    std::cout << "Cleaning up thread-local data: " << *(int*)value << std::endl;
    delete (int*)value;
}

void* thread_func(void* arg) {
    int* value = new int(42);
    pthread_setspecific(tls_key, value);
    std::cout << "Thread-local value: " << *(int*)pthread_getspecific(tls_key) << std::endl;
    return nullptr;
}
int main() {
    pthread_key_create(&tls_key, cleanup);
    // 创建多个线程并运行
    const int num_threads = 10;
    pthread_t threads[num_threads];
    
    for (int i = 0; i < num_threads; ++i) {
        pthread_create(&threads[i], nullptr, thread_func, nullptr);
    }
    
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_key_delete(tls_key);
    return 0;
}