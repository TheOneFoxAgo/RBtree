#ifndef QUEUE_ARRAY
#define QUEUE_ARRAY
#include <cstddef>
#include "Queue.h"
template <class T>
class QueueArray : public Queue<T> {
public:
  QueueArray(std::size_t capacity) : head_(0), tail_(0), capacity_(capacity + 1)
  {
    if (capacity == 0 || capacity == ~0) {
      throw WrongQueueSize();
    }
    pointer_ = new T[capacity_];
  }
  ~QueueArray() override {
    delete[] pointer_;
  }
  
  QueueArray(const QueueArray&) = delete;
  QueueArray(QueueArray&&) noexcept = delete;
  QueueArray& operator=(const QueueArray&) = delete;
  QueueArray& operator=(QueueArray&&) noexcept = delete;
  
  void enQueue(const T& data) override
  {
    if (cycleForward(tail_) == head_) {
      throw QueueOverflow();
    }
    pointer_[tail_] = data;
    tail_ = cycleForward(tail_);
  }
  T deQueue() override
  {
    if (isEmpty()) {
      throw QueueUnderflow();
    }
    T& temp = pointer_[head_];
    head_ = cycleForward(head_);
    return temp;
  }
  bool isEmpty() const override
  {
    return head_ == tail_;
  }

private:
  std::size_t cycleForward(std::size_t index)
  {
    ++index;
    return index == capacity_ ? 0 : index;
  }

  std::size_t head_;
  std::size_t tail_;
  std::size_t capacity_;
  T* pointer_;
};
#endif
