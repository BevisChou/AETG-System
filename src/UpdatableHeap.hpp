#include <cstdint>
#include <map>
#include <exception>

template<typename T>
class HeapItem{
public:
    HeapItem();
    HeapItem(uint32_t);
    HeapItem(T, uint32_t);
    HeapItem(HeapItem<T>&&);
    HeapItem<T>& operator=(HeapItem<T>&&);
    bool operator>(const HeapItem<T>&);
    void increment(uint32_t);
    T store();
    uint32_t value();
private:
    T store_;
    uint32_t value_;
};

template<typename T>
class UpdatableHeap{
public:
    UpdatableHeap();
    ~UpdatableHeap();
    void push_or_update(T, uint32_t);
    void pop();
    T top();
    uint32_t get_value(T);
    uint32_t size();
private:
    void resize(uint32_t);
    void percolate_up(uint32_t);
    void percolate_down(uint32_t);
    uint32_t capacity_;
    uint32_t size_;
    HeapItem<T>* heap_;
    std::map<T, uint32_t> locs_;
};

template<typename T>
UpdatableHeap<T>::UpdatableHeap() :
    capacity_(1),
    size_(0)
{
    heap_ = new HeapItem<T>[capacity_ + 1];
    heap_[0] = std::move(HeapItem<T>(UINT32_MAX));
}

template<typename T>
UpdatableHeap<T>::~UpdatableHeap()
{
    delete[] heap_;
}

template<typename T>
void UpdatableHeap<T>::push_or_update(T item, uint32_t value)
{
    auto itor = locs_.find(item);
    if(itor != locs_.end())
    {
        uint32_t loc = itor->second;
        heap_[loc].increment(value);
        percolate_up(loc);
    }
    else
    {
        if(size_ == capacity_)
        {
            resize(capacity_ * 2);
        }
        size_++;
        heap_[size_] = std::move(HeapItem<T>(item, value));
        locs_[item] = size_;
    }
}

template<typename T>
void UpdatableHeap<T>::pop()
{
    if(size_ == 0)
    {
        throw std::exception();
    }
    size_--;
    heap_[1] = std::move(heap_[size_]);
    percolate_down(1);
}

template<typename T>
T UpdatableHeap<T>::top()
{
    if(size_ == 0)
    {
        throw std::exception();
    }
    return heap_[1].store();
}

template<typename T>
uint32_t UpdatableHeap<T>::get_value(T item)
{
    auto itor = locs_.find(item);
    if(itor != locs_.end())
    {
        return heap_[itor->second].value();
    }
    else return 0;
}

template<typename T>
uint32_t UpdatableHeap<T>::size()
{
    return size_;
}

template<typename T>
void UpdatableHeap<T>::resize(uint32_t capacity)
{
    if(capacity < size_)
    {
        throw std::exception();
    }
    capacity_ = capacity;
    HeapItem<T>* heap = new HeapItem<T>[capacity_ + 1];
    for(uint32_t i = 0; i <= size_; i++)
    {
        heap[i] = std::move(heap_[i]);
    }
    delete[] heap_;
    heap_ = heap;
}

template<typename T>
void UpdatableHeap<T>::percolate_up(uint32_t loc)
{
    HeapItem<T> item(std::move(heap_[loc]));
    while(item > heap_[loc / 2])
    {
        heap_[loc] = std::move(heap_[loc / 2]);
        locs_[heap_[loc].store()] = loc;
        loc /= 2;
    }
    heap_[loc] = std::move(item);
    locs_[heap_[loc].store()] = loc;
}

template<typename T>
void UpdatableHeap<T>::percolate_down(uint32_t loc)
{
    uint32_t next_loc;
    HeapItem<T> item(std::move(heap_[loc]));
    while((next_loc = loc * 2) <= size_)
    {
        next_loc += (next_loc + 1 <= size && heap_[next_loc + 1] > heap_[next_loc]) ? 1 : 0;
        if(item > heap_[next_loc]) break;

        heap_[loc] = std::move(heap_[next_loc]);
        locs_[heap_[loc].store()] = loc;
        loc = next_loc;
    }
    heap_[loc] = std::move(item);
    locs_[heap_[loc].store()] = loc;
}

template<typename T>
HeapItem<T>::HeapItem() :
    HeapItem(0) {}

template<typename T>
HeapItem<T>::HeapItem(uint32_t value) :
    value_(value) {}

template<typename T>
HeapItem<T>::HeapItem(T store, uint32_t value) :
    store_(store),
    value_(value) {}

template<typename T>
HeapItem<T>::HeapItem(HeapItem<T>&& other) :
    store_(std::move(other.store_)),
    value_(other.value_) {}

template<typename T>
HeapItem<T>& HeapItem<T>::operator=(HeapItem<T>&& other)
{
    if(this != &other)
    {
        store_ = std::move(other.store_);
        value_ = other.value_;
    }
    return *this;
}

template<typename T>
bool HeapItem<T>::operator>(const HeapItem<T>& other)
{
    return value_ >= other.value_;
}

template<typename T>
void HeapItem<T>::increment(uint32_t delta)
{
    value_ += delta;
}

template<typename T>
T HeapItem<T>::store()
{
    return store_;
}

template<typename T>
uint32_t HeapItem<T>::value()
{
    return value_;
}