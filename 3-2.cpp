/*
 * Реализовать дек с динамическим зацикленным буфером.
 * Обрабатывать команды push * и pop *.
 */

#include <cassert>
#include <iostream>

template <typename T> class Deque {
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

  std::size_t end() {
    auto end = start + real_size;
    if (end > capacity())
      end %= capacity();
    return end;
  }

public:
  Deque() : buffer(nullptr), start(0), real_size(0), buffer_size(0) {}

  Deque(const Deque &) = delete;
  Deque &operator=(const Deque &) = delete;

  std::size_t size() { return real_size; }
  std::size_t capacity() { return buffer_size; }

  void grow(std::size_t newSize) {
    assert(newSize >= size());
    if (newSize == capacity())
      return;

    T *memory = new T[newSize];

    if (start >= end()) {
      std::swap_ranges(buffer + start, buffer + buffer_size, memory);
      std::swap_ranges(buffer, buffer + end(), memory + buffer_size - start);
    } else {
      std::swap_ranges(buffer + start, buffer + end(), memory);
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

  template <typename U> void push_back(U &&value) {
    if (size() == capacity())
      reserve(1);
    buffer[(real_size + start) % capacity()] = std::forward<U>(value);
    real_size++;
  }

  template <typename U> void push_front(U &&value) {
    if (size() == capacity())
      reserve(1);
    std::size_t insertion_point = (start == 0 ? capacity() : start) - 1;
    buffer[insertion_point] = std::forward<U>(value);
    start = insertion_point;
    real_size++;
  }

  T front() {
    assert(size() > 0);
    return buffer[start];
  }

  T back() {
    assert(size() > 0);
    return buffer[end() - 1];
  }

  void pop_front() {
    assert(size() > 0);
    start = (start + 1) % buffer_size;
    real_size--;

    if (2 * size() <= capacity())
      shrink();
  }

  void pop_back() {
    assert(size() > 0);
    real_size--;

    if (2 * size() <= capacity())
      shrink();
  }

  ~Deque() { delete[] buffer; }

  void debug() {
    std::cout << "start = " << start << ", end = " << end()
              << ", size = " << size() << ", capacity = " << capacity()
              << ", elements = {";
    if (buffer == nullptr || size() == 0)
      std::cout << "null";
    else {
      for (int i = 0; i < capacity() - 1; i++)
        std::cout << buffer[i] << ", ";
      std::cout << buffer[capacity() - 1];
    }
    std::cout << "}" << std::endl;
  }
};

bool check_deque(int operation, int value, Deque<int> &deque) {
  switch (operation) {
  case 3:
    deque.push_back(value);
    return true;
  case 1:
    deque.push_front(value);
    return true;
  case 2: {
    if (value == -1)
      return deque.size() == 0;
    if (deque.size() == 0)
      return false;
    auto actual = deque.front();
    deque.pop_front();
    if (actual != value)
      return false;
    return true;
  }
  case 4:
    if (value == -1)
      return deque.size() == 0;
    if (deque.size() == 0)
      return false;
    auto actual = deque.back();
    deque.pop_back();
    if (actual != value)
      return false;
    return true;
  }

  return false;
}

int main() {
  int n;
  std::cin >> n;
  Deque<int> deque;

  for (int i = 0; i < n; i++) {
    int a, b;
    std::cin >> a >> b;

    if (!check_deque(a, b, deque)) {
      std::cout << "NO" << std::endl;
      return 0;
    }
  }

  std::cout << "YES" << std::endl;
}