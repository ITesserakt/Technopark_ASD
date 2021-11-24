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

  void pop_last() {
    assert(size() > 0);
    len--;
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