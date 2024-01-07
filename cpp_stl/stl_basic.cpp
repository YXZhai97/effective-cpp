#include <iostream>
#include <vector>
#include <cstddef>
#include <new>

// C++11 allocator with debug output
template<class Tp>
struct NAlloc
{
    typedef Tp value_type;
 
    NAlloc() = default;
    template<class T>
    NAlloc(const NAlloc<T>&) {}
 
    Tp* allocate(std::size_t n)
    {
        // calculate mem to allocate 
        n *= sizeof(Tp);
        std::cout << "type size: " << sizeof(Tp) <<std::endl;
        Tp* p = static_cast<Tp*>(::operator new(n));
        std::cout << "allocating " << n << " bytes @ " << p << '\n';
        return p;
    }
 
    void deallocate(Tp* p, std::size_t n)
    {
        std::cout << "deallocating " << n * sizeof *p << " bytes @ " << p << "\n\n";
        ::operator delete(p);
    }
};

template<class T, class U>
bool operator==(const NAlloc<T>&, const NAlloc<U>&) { return true; }
 
template<class T, class U>
bool operator!=(const NAlloc<T>&, const NAlloc<U>&) { return false; }

class MyData{
    public:
        MyData(int data):data_(data) {
            std::cout<< "Construct Mydata " << data_ <<std::endl;
        }
        ~MyData() {
            std::cout << "Deconstruct MyData " << data_ << std::endl;
        }
    private:
        int data_;
};

void test_vector() {
    // Create a vector containing integers
    std::vector<int> v;
    v.reserve(5);
    std::cout << "vector initial capacity: " << v.capacity() <<std::endl;
    v = {8, 4, 5, 9};
    v.push_back(6);
    v.push_back(9);
    std::cout << "vector capacity: " << v.capacity() <<std::endl;


    // Overwrite element at position 2
    v[2] = -1;
    auto i = v.at(2);
    std::cout << "element at position " << 2 << " is "<< i << std::endl;
    auto last_element = v.back();
    std::cout<< "last element: " << last_element << std::endl;
    // Print out the vector
    for (int n : v)
        std::cout << n << ' ';
    std::cout << '\n';


}

void test_vector_allocate(){


    constexpr int max_elements = 32;
    std::cout << "using reserve: \n";
    {
        std::vector<MyData, NAlloc<MyData>> v1;
        v1.clear();
        v1.reserve(max_elements); // reserves at least max_elements * sizeof(int) bytes
 
        for (int n = 0; n < max_elements; ++n)
            v1.emplace_back(n);
    }
    std::cout << "end using resrve: \n";
 
    std::cout << "not using reserve: \n";
    {
        std::vector<MyData, NAlloc<MyData>> v1;

        // 当元素个数达到 capacity 时会先分配新的内存再拷贝构造，最后释放老的内存
        for (int n = 0; n < max_elements; ++n)
        {
            if (v1.size() == v1.capacity())
                std::cout << "size() == capacity() == " << v1.size() << '\n';
            v1.emplace_back(n);
        }
    }
    std::cout << "end not using reserve \n";
}




int main()
{
    test_vector();
    test_vector_allocate();
}