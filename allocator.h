#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include <memory>
#include <vector>
#include <map>
#include <optional>

struct deleter
{
    void operator()(void* ptr)
    {
        ::operator delete(ptr);
    }
};

template <typename T, std::size_t ReservBlockSize = 100>
class PoolAllocator 
{
public:
    PoolAllocator()
    {
        allocateBlock();
    }

    template <class U> 
    PoolAllocator(const PoolAllocator<U>& other) noexcept
    {
        other.pool.clear();
        std::copy(_pool.cbegin(), _pool.cend(), std::back_inserter(other._pool));
    }

    PoolAllocator select_on_container_copy_construction() const
    {
        return PoolAllocator();
    }

    T* allocate(std::size_t n)
    {
        if (_currIndex + n > ReservBlockSize) 
        {
            std::optional<T*> block = eraseFreeBlock(n);
            if (block)
            {
                return block.value();
            }
            else
            {
                allocateBlock(n + ReservBlockSize);
            }
        }

        T* result = _currBlock + _currIndex;
        _currIndex += n;
        
        return result;
    }

    void deallocate(T* p, std::size_t n) 
    {
        for (std::size_t i = 0; i < n; ++i) 
        {
            p[i].~T();
        }
    }

private:
    void allocateBlock(std::size_t size = ReservBlockSize)
    {
        if (_currIndex < ReservBlockSize && !_pool.empty())
            _freeBlocks.emplace(ReservBlockSize - _currIndex, _pool.back().get() + _currIndex);
        
        _currIndex = 0;
        _pool.push_back(std::shared_ptr<T>{ reinterpret_cast<T*>(::operator new(size * sizeof(T))), deleter() });
        _currBlock = _pool.back().get();
    }

    std::optional<T*> eraseFreeBlock(std::size_t size)
    {
        auto blockIt = _freeBlocks.lower_bound(size);

        if (blockIt == _freeBlocks.end())
            return std::optional<T*>();

        auto [blockSize, block] = *blockIt;
        _freeBlocks.erase(blockIt);
        if (blockSize > size)
            _freeBlocks.emplace(blockSize - size, block + size);
        
        return block;
    }

private:
    std::size_t _currIndex = 0;
    T* _currBlock = nullptr;
    
    std::vector<std::shared_ptr<T>> _pool;
    std::multimap<std::size_t, T*> _freeBlocks;

public:
    template<class U>
    struct rebind
    {
        typedef PoolAllocator<U, ReservBlockSize> other;
    };

    using value_type = T;
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;
    
};

template <class T, std::size_t TReservBlockSize, class U, std::size_t UReservBlockSize>
constexpr bool operator== (const PoolAllocator<T, TReservBlockSize>& a1, const PoolAllocator<U, UReservBlockSize>& a2) noexcept
{
    return a1.pool == a2.pool;
}

template <class T, std::size_t TReservBlockSize, class U, std::size_t UReservBlockSize>
constexpr bool operator!= (const PoolAllocator<T, TReservBlockSize>& a1, const PoolAllocator<U, UReservBlockSize>& a2) noexcept
{
    return a1.pool != a2.pool;
}

#endif