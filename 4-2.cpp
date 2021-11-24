/*
 * Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор
 * пользователя, посещаемость сайта). Напишите программу, которая выбирает K
 * пользователей, которые чаще других заходили на сайт, и выводит их в порядке
 * возрастания посещаемости. Количество заходов и идентификаторы пользователей
 * не повторяются.
 */

#include <cassert>
#include <iostream>

template <typename T, typename Comparator = std::less<T>> class Heap {
private:
  T *buffer;
  std::size_t buffer_size;
  std::size_t index;
  Comparator compare;

  std::size_t next_power_of_two(std::size_t n) {
    std::size_t result = 1;
    while (n > result)
      result <<= 1;
    return result;
  };

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
    assert(size() > 0);
    std::swap(buffer[1], buffer[index]);
    index--;
    normalize();
  }

  void pop_last() {
    assert(size() > 0);
    index--;
  }
};

struct User {
  int id;
  int attendance;

  bool operator<(const User &rhs) const { return attendance < rhs.attendance; }

  friend std::istream &operator>>(std::istream &is, User &user) {
    int id, attendance;
    is >> id >> attendance;
    user = {id, attendance};
    return is;
  }
};

int main() {
  int n, k;
  std::cin >> n >> k;

  User *users = new User[n];
  for (int i = 0; i < n; i++)
    std::cin >> users[i];

  Heap<User> heap;
  heap.grow(k + 1);

  for (int i = 0; i < k; i++)
    heap.insert(users[i]);

  for (int i = k; i < n; i++) {
    auto min = heap.top();
    if (users[i] < min) {
      heap.insert(users[i]);
      heap.pop_last();
    }
    assert(heap.size() < k + 1);
  }

  for (int i = 0; i < k; i++) {
    std::cout << heap.top().id << ' ';
    heap.pop();
  }
}