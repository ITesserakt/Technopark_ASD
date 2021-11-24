/*
 * Напишите программу, которая использует кучу для слияния K отсортированных
 * массивов суммарной длиной N.
 */

#include <cassert>
#include <iostream>

template <typename T, typename Comparator = std::less<T>> class Heap {
private:
  T *buffer;
  std::size_t cap;
  std::size_t len;
  Comparator compare;

  std::size_t next_power_of_two(std::size_t n) {
    std::size_t result = 1;
    while (n > result)
      result <<= 1;
    return result;
  };

  void sift_up(std::size_t index) {
    while (index > 0) {
      auto parent = (index - 1) / 2;
      if (compare(buffer[parent], buffer[index]))
        return;
      std::swap(buffer[index], buffer[parent]);
      index = parent;
    }
  }

  void sift_down(std::size_t index) {
    while (index < size()) {
      auto left = index * 2 + 1;
      auto right = index * 2 + 2;
      auto next = index;
      if (left < size() && compare(buffer[left], buffer[next]))
        next = left;
      if (right < size() && compare(buffer[right], buffer[next]))
        next = right;
      if (next == index)
        return;
      std::swap(buffer[next], buffer[index]);
      index = next;
    }
  }

public:
  explicit Heap(Comparator compare = Comparator())
      : cap(0), len(0), compare(compare), buffer(nullptr) {}

  ~Heap() { delete[] buffer; }

  Heap(const Heap &) = delete;
  Heap &operator=(const Heap &) = delete;

  std::size_t size() { return len; }
  std::size_t capacity() { return cap; }

  void grow(std::size_t newSize) {
    assert(newSize >= size());
    if (newSize != capacity()) {
      T *memory = new T[newSize];
      if (buffer != nullptr) {
        std::swap_ranges(buffer, buffer + size(), memory);
        delete[] buffer;
      }
      buffer = memory;
    }
    cap = newSize;
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

  template <typename U> void insert(U &&value) {
    if (size() == capacity())
      reserve(1);
    buffer[len++] = std::forward<U>(value);
    sift_up(size() - 1);
  }

  T top() {
    assert(size() > 0);
    return buffer[0];
  }

  void pop() {
    len--;
    if (size() > 0) {
      std::swap(buffer[0], buffer[size()]);
      sift_down(0);
    }
  }

  void debug() {
    std::cout << "size = " << size() << ", capacity = " << capacity()
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

template <typename T> struct SortingNode {
  T element;
  int from_array;
  int next_element;

  bool operator<(const SortingNode &rhs) const { return element < rhs.element; }

  friend std::ostream &operator<<(std::ostream &os, const SortingNode &node) {
    return os << node.element;
  }
};

int main() {
  int n, sum = 0;
  std::cin >> n;
  int **arrays = new int *[n];
  int *array_sizes = new int[n];

  for (int i = 0; i < n; i++) {
    int m;
    std::cin >> m;
    int *array = new int[m];
    for (int j = 0; j < m; j++)
      std::cin >> array[j];

    array_sizes[i] = m;
    arrays[i] = array;
    sum += m;
  }

  Heap<SortingNode<int>> heap;
  heap.reserve_exact(n);

  for (int i = 0; i < n; i++) {
    heap.insert(SortingNode<int>{arrays[i][0], i, 1});
  }

  for (int i = 0; i < sum; i++) {
    auto min = heap.top();
    std::cout << min.element << ' ';

    heap.pop();
    if (min.next_element < array_sizes[min.from_array]) {
      min.element = arrays[min.from_array][min.next_element++];
      heap.insert(min);
    }
  }

  for (int i = 0; i < n; i++)
    delete[] arrays[i];
  delete[] arrays;
  delete[] array_sizes;
}