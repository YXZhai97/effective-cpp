#include <iostream>

/**
 * 模版类的static成员变量每个模版有一份
*/
template <typename T>
class MyClass {
public:
    static int static_var;
    static int static_int_var;

    void increment() {
        ++static_var;
        ++static_int_var;
    }

    void print() {
        std::cout << "Static variable for " << typeid(T).name() << ": " << static_var << std::endl;
        std::cout << "Static int_variable for " << typeid(T).name() << ": " << static_var << std::endl;
    }
};

// 定义静态变量
template <typename T>
int MyClass<T>::static_var = 0;

template <typename T>
int MyClass<T>::static_int_var = 0;


/**
 * 非模版类的 static成员变量只有一份
*/
class MyClassTwo {
public:
    static int static_var;
    static int static_int_var;

    void increment() {
        ++static_var;
        ++static_int_var;
    }

    void print() {
        std::cout << "Static variable " << ": " << static_var << std::endl;
        std::cout << "Static int_variable " << ": " << static_int_var << std::endl;
    }
};

int MyClassTwo::static_int_var = 1;
int MyClassTwo::static_var = 0;

int main() {
    MyClass<int> intInstance;
    MyClass<double> doubleInstance;

    MyClassTwo instance;
    instance.print();
    instance.increment();
    instance.print();

    MyClassTwo instance2;
    instance2.print();

    intInstance.increment();
    intInstance.print(); // Output: Static variable for int: 1

    doubleInstance.increment();
    doubleInstance.print(); // Output: Static variable for double: 1

    intInstance.increment();
    intInstance.print(); // Output: Static variable for int: 2

    doubleInstance.print(); // Output: Static variable for double: 1

    return 0;
}
