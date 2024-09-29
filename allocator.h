#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <cstddef>
#include <memory>
#include <vector>

template<typename T, size_t InitSize = 100>
class MyAllocator
{
public:
    using value_type = T;

    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap = std::true_type;
    
    struct deleter
    {
        void operator() (void* ptr)
        {
            ::operator delete(ptr);
        }
    };

public:
    MyAllocator()
        _pools{ {::operator new (sizeof(std::byte) * InitSize), deleter()} }
    {}

    template <class U>
    MyAllocator(const MyAllocator<U>& other) noexcept 
    {
        _poold = other._pools;
    }

    MyAllocator select_on_container_copy_construction() const
    {
        return MyAllocator();
    }

    value_type* allocate(std::size_t n)
    {
        
    }

    void deallocate(value_type* p, std::size_t n)
    {
        
    }

private:
    std::vector<std::shared_ptr<void>> _pools;
    size_t _reservedMem = 0;

};

#endif