#ifndef QUEUE
#define QUEUE
#include <stdexcept>
template <class T> class Queue {
public:
  virtual ~Queue() {}
  virtual void enQueue(const T &e) = 0;
  virtual T deQueue() = 0;
  virtual bool isEmpty() const = 0;
};

class QueueOverflow : public std::exception {
  const char *what() const noexcept override { return "Queue is full\n"; }
};
class QueueUnderflow : public std::exception {
  const char *what() const noexcept override { return "Queue is empty\n"; }
};
class WrongQueueSize : public std::exception {
  const char *what() const noexcept override { return "Wrong queue size\n"; }
};
#endif
