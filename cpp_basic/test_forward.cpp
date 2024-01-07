#include<iostream>
#include<utility>

// L-value input template func
template<typename T>
void print(T &t) {
    std::cout << "Lvalue ref" << std::endl;
}

// R-value input template func
template<typename T>
void print(T &&t) {
    std::cout << "Rvalue ref" << std::endl;
}

// All-type input fuc
template<typename T>
void testForward(T &&v) {
    print(v); // v此时已经是个左值了,永远调用左值版本的print
    print(std::forward<T>(v)); // 转发
    print(std::move(v)); // move将左值转换为右值，永远调用右值版本的print

    std::cout << "==========end test============" << std::endl;
}

int main(int argc, char *argv[]) {
    int x = 1; // x is L value
    testForward(x); // L value as input
    testForward(std::move(x)); // R value as input
    testForward(100); // R value as input
}