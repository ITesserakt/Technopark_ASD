/*
 * Напишите программу, которая использует кучу для слияния K отсортированных
 * массивов суммарной длиной N.
 */

#include <cassert>
#include <iostream>

template <typename T, typename Comparator = std::less<T>> class Heap {
private:
  T *buffer;
  std::size_t buffer_size;
  std::size_t index;
  Comparator compare;

public:
  explicit Heap(Comparator compare = Comparator())
      : buffer_size(0), index(0), compare(compare), buffer(nullptr) {
    reserve_exact(2);
  }

  ~Heap() { delete[] buffer; }

  Heap(const Heap &) = delete;
  Heap &operator=(const Heap &) = delete;

  std::size_t size() { return index; }
  std::size_t capacity() { return buffer_size; }

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
    buffer_size = newSize;
  }

  void reserve(std::size_t additional) {
    if (capacity() - size() >= additional)
      return;
    constexpr auto next_power_of_two = [](std::size_t n) {
      std::size_t result = 1;
      while (n > result)
        result <<= 1;
      return result;
    };
    grow(next_power_of_two(size() + additional));
  }

  void reserve_exact(std::size_t additional) {
    if (capacity() - size() >= additional)
      return;
    grow(size() + additional);
  }

  void sift() {
    auto inserted = index;

    while (true) {
      if (inserted == 1)
        break;
      auto parent = inserted / 2;

      if (compare(buffer[parent], buffer[inserted]))
        break;
      std::swap(buffer[parent], buffer[inserted]);
      inserted = parent;
    }
  }

  template <typename U> void insert(U &&value) {
    if (++index == buffer_size)
      reserve(1);
    buffer[index] = std::forward<U>(value);
    sift();
  }

  void normalize() {
    std::size_t parent = 1;
    while (true) {
      std::size_t child_left = parent * 2;
      std::size_t child_right = parent * 2 + 1;
      if (child_left > index)
        break;
      if (child_right <= index &&
          !compare(buffer[child_left], buffer[child_right]))
        std::swap(buffer[child_left], buffer[child_right]);

      if (compare(buffer[parent], buffer[child_left]))
        break;
      else
        std::swap(buffer[parent], buffer[child_left]);
      parent = child_left;
    }
  }

  T top() {
    assert(size() > 0);
    return buffer[1];
  }

  void pop() {
    std::swap(buffer[1], buffer[index]);
    index--;
    normalize();
  }
};

template <typename T> struct SortingNode {
  T element;
  int from_array;
  int next_element;

  bool operator<(const SortingNode &rhs) const { return element < rhs.element; }
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
  heap.grow(n + 1);

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