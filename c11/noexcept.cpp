#include <iostream> 
void may_throw();
void may_throw1() noexcept(false);
void no_throw() noexcept;
auto lmay_throw = []{};
auto lmay_throw1 = []() noexcept(false) {};
auto lno_throw = []() noexcept {};
void BlockThrow() noexcept(true) {
    std::cout <<"BlickThrow func" << '\n';
    throw 1;
}

void BlockNoThrow() noexcept(false) {
    std::cout << "BlockNoThrow func" << '\n'; 
    throw 2;
}

int main() {

    // check func noexcept status
    std::cout << std::boolalpha
              << "Is may_throw() noexcept? " << noexcept(may_throw()) << '\n'
              << "Is may_throw1() noexcept? " << noexcept(may_throw1()) << '\n'
              << "Is no_throw() noexcept? " << noexcept(no_throw()) << '\n'
              << "Is lmay_throw() noexcept? " << noexcept(lmay_throw()) << '\n'
              << "Is lmay_throw1() noexcept? " << noexcept(lmay_throw1()) << '\n'
              << "Is lno_throw() noexcept? " << noexcept(lno_throw()) << '\n';
    // will ctach
    try {
        BlockNoThrow();
    } catch (...) {
        std::cout << "BlockNoThrow catch exception" << std::endl;
    }
    // not catch
    try {
        BlockThrow(); // because noexcept decleared, system will not throw, direct abort
    } catch(...) {
        std::cout << "BlockThrow Found throw" << std::endl;
    }
}
