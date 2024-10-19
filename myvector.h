#ifndef MYVECTOR_H
#define MYVECTOR_H

#include <memory>

template<typename T, typename Allocator = std::allocator<T>>
class MyVector
{
public:
    using value_type = T;

public:
    MyVector() = default;

    MyVector(std::initializer_list<T> values) :
        _size(values.size())
    {
        reserve(_size * 2 + 1);
        std::copy(values.begin(), values.end(), _data);
    }

    ~MyVector() 
    {
        clear();
        std::allocator_traits<Allocator>::deallocate(_alloc, _data, _capacity);
    }

    void push_back(const T& val)
    {
        if (_size == _capacity)
            reserve(_capacity * 2 + 1);

        std::allocator_traits<Allocator>::construct(_alloc, _data + _size, val);
        ++_size;
    }

    std::size_t size() const { return _size; }

    bool empty() const { return _size == 0; }

    void clear() 
    {
        for (std::size_t i = 0; i < _size; ++i)
        {
            std::allocator_traits<Allocator>::destroy(_alloc, _data + i);
        }

        _size = 0;
    }

    void reserve(std::size_t newCapacity)
    {
        T* newData = std::allocator_traits<Allocator>::allocate(_alloc, newCapacity);
        std::copy(_data, _data + _capacity, newData);
        std::swap(newData, _data);
        std::allocator_traits<Allocator>::deallocate(_alloc, newData, _capacity);
        _capacity = newCapacity;
    }

public:
    class iterator
    {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        iterator(pointer ptr) : _ptr(ptr) {}

        reference operator*() const { return *_ptr; }
        pointer operator->() { return _ptr; }

        iterator& operator++() 
        {
            _ptr++;
            return *this;
        }

        bool operator==(const iterator& other) const { return _ptr == other._ptr; }
        bool operator!=(const iterator& other) const { return _ptr != other._ptr; }

    private:
        pointer _ptr;

    };
 
    iterator begin() { return iterator{ _data }; }

    iterator end() { return iterator{ _data + _size }; }

private:
    std::size_t _capacity = 0;
    std::size_t _size = 0;
    T* _data = nullptr;

    Allocator _alloc;

};

#endif