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
  std::size_t end;

public:
  Queue() : buffer(nullptr), start(0), end(0) {}

  void grow(std::size_t newSize) { assert(newSize >) }
};

int main() {}