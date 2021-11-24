/*
 * Реализовать очередь с динамическим зацикленным буфером.
 * Обрабатывать команды push back и pop front.
 */

#include <cassert>
#include <iostream>

template <typename T> class Queue {
private:
  T *buffer;
  std::size_t start;
  std::size_t buffer_size;
  std::size_t real_size;

  std::size_t next_power_of_two(std::size_t n) {
    std::size_t result = 1;
    while (n > result)
      result <<= 1;
    return result;
  };

public:
  Queue() : buffer(nullptr), start(0), real_size(0), buffer_size(0) {}

  Queue(const Queue &) = delete;
  Queue &operator=(const Queue &) = delete;

  std::size_t size() { return real_size; }
  std::size_t capacity() { return buffer_size; }

  void grow(std::size_t newSize) {
    assert(newSize >= size());
    if (newSize == capacity())
      return;

    T *memory = new T[newSize];
    auto end = start + real_size;
    if (end > buffer_size)
      end %= buffer_size;

    if (start >= end) {
      std::swap_ranges(buffer + start, buffer + buffer_size, memory);
      std::swap_ranges(buffer, buffer + end, memory + buffer_size - start);
    } else {
      std::swap_ranges(buffer + start, buffer + end, memory);
    }

    delete[] buffer;
    buffer = memory;
    start = 0;
    buffer_size = newSize;
  }

  void reserve(std::size_t additional) {
    if (capacity() - size() >= additional)
      return;
    grow(next_power_of_two(size() + additional));
  }

  void reserve_exact(std::size_t additional) {
    if (capacity() - size() >= additional)
      return;
    grow(size() + additional);
  }

  void shrink() {
    if (capacity() > size())
      grow(size());
  }

  template <typename U> void push(U &&value) {
    if (size() == capacity())
      reserve(1);
    buffer[(real_size + start) % capacity()] = std::forward<U>(value);
    real_size++;
  }

  T top() {
    assert(size() > 0);
    return buffer[start];
  }

  void pop() {
    assert(size() > 0);
    buffer[start] = 0;
    start = (start + 1) % buffer_size;
    real_size--;

    if (2 * size() <= capacity())
      shrink();
  }

  ~Queue() { delete[] buffer; }

  void debug() {
    std::cout << "start = " << start << ", end = "
              << (capacity() == 0 ? 0 : (start + real_size) % capacity())
              << ", size = " << size() << ", capacity = " << capacity()
              << ", elements = {";
    if (buffer == nullptr)
      std::cout << "null";
    else {
      for (int i = 0; i < capacity() - 1; i++)
        std::cout << buffer[i] << ", ";
      std::cout << buffer[capacity() - 1];
    }
    std::cout << "}" << std::endl;
  }
};

bool check_queue(int operation, int value, Queue<int> &queue) {
  switch (operation) {
  case 3:
    queue.push(value);
    return true;
  case 2:
    if (value == -1)
      return queue.size() == 0;
    if (queue.size() == 0)
      return false;
    auto actual = queue.top();
    queue.pop();
    if (actual != value)
      return false;
    return true;
  }
  return false;
}

int main() {
  int n;
  std::cin >> n;
  Queue<int> q;

  for (int i = 0; i < n; i++) {
    int a, b;
    std::cin >> a >> b;

    if (!check_queue(a, b, q)) {
      std::cout << "NO" << std::endl;
      return 0;
    }
  }

  std::cout << "YES" << std::endl;
}