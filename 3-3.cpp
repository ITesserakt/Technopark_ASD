/*
 * Реализовать очередь с помощью двух стеков. Использовать стек, реализованный с
 * помощью динамического буфера. Обрабатывать команды push back и pop front.
 */

#include <cassert>
#include <iostream>

template <typename T, bool auto_shrink = true> class Stack {
private:
  T *buffer;
  std::size_t len;
  std::size_t cap;

  std::size_t next_power_of_two(std::size_t n) {
    std::size_t result = 1;
    while (n > result)
      result <<= 1;
    return result;
  };

public:
  Stack() : len(0), buffer(nullptr), cap(0) {}

  Stack(const Stack &) = delete;
  Stack &operator=(const Stack &) = delete;

  ~Stack() { delete[] buffer; }

  auto size() const { return len; }
  auto capacity() const { return cap; }

  void grow(std::size_t new_size) {
    assert(new_size >= size());
    if (new_size == capacity())
      return;

    T *memory = new T[new_size];
    std::swap_ranges(buffer, buffer + len, memory);
    delete[] buffer;
    buffer = memory;
    cap = new_size;
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
    buffer[len++] = std::forward<U>(value);
  }

  T top() {
    assert(size() > 0);
    return buffer[len - 1];
  }

  void pop() {
    assert(size() > 0);
    len--;

    if constexpr (auto_shrink) {
      if (2 * size() <= capacity())
        shrink();
    }
  }

  void debug() {
    std::cout << "size = " << size() << ", capacity = " << capacity()
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

template <typename T> class Queue {
private:
  Stack<T, false> in;
  Stack<T> out;

public:
  auto size() const { return in.size() + out.size(); }
  auto capacity() const { return in.capacity() + out.capacity(); }

  template <typename U> void push(U &&value) {
    in.template push(std::forward<U>(value));
  }

  T front() {
    if (out.size() == 0)
      while (in.size() != 0) {
        out.template push(in.top());
        in.pop();
      }
    return out.top();
  }

  void pop() {
    if (out.size() == 0)
      while (in.size() != 0) {
        out.template push(in.top());
        in.pop();
      }
    out.pop();
  }

  void debug() {
    std::cout << "input stack: " << std::endl;
    in.debug();
    std::cout << "output stack: " << std::endl;
    out.debug();
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
    auto actual = queue.front();
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