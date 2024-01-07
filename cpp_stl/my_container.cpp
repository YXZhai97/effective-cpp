#include <iostream>
#include <vector>
#include <iterator>
#include <type_traits>

template <typename T>
class MyIterator {
private:
    T* ptr;

public:
    MyIterator(T* p) : ptr(p) {}

    T& operator*() const {
        return *ptr;
    }

    MyIterator& operator++() {
        ++ptr;
        return *this;
    }

    MyIterator& operator+=(std::ptrdiff_t n) {
        ptr += n;
        return *this;
    }

    bool operator!=(const MyIterator& other) const {
        return ptr != other.ptr;
    }

    // Adding type traits required for std::next
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;
    using iterator_category = std::random_access_iterator_tag;
};

template <typename T>
class MyContainer {
private:
    T* data;
    size_t size;

public:
    MyContainer(T* arr, size_t s) : data(arr), size(s) {}

    MyIterator<T> begin() const {
        return MyIterator<T>(data);
    }

    MyIterator<T> end() const {
        return MyIterator<T>(data + size);
    }
};

int main() {
    int arr[] = {1, 2, 3, 4, 5};
    MyContainer<int> myContainer(arr, 5);
    auto it = myContainer.begin();
    // Using std::next with an offset of 1 to move the iterator to the next position
    auto nextIt = std::next(it, 1);
    std::cout << "Current value: " << *it << std::endl;
    std::cout << "Value after moving one position: " << *nextIt << std::endl;
    return 0;
}



