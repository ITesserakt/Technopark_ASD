/*
 * Имеется лог-файл, в котором хранятся пары для N пользователей (Идентификатор
 * пользователя, посещаемость сайта). Напишите программу, которая выбирает K
 * пользователей, которые чаще других заходили на сайт, и выводит их в порядке
 * возрастания посещаемости. Количество заходов и идентификаторы пользователей
 * не повторяются.
 */

#include <cassert>
#include <iostream>
#include <vector>
//#include <rapidcheck.h>

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

  template <typename C>
  explicit Heap(Heap<T, C> &&other, Comparator compare = Comparator())
      : Heap(compare) {
    while (other.size() > 0) {
      insert(other.top());
      other.pop();
    }
  }

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

struct User {
  int id;
  int attendance;

  bool operator>(const User &rhs) const { return attendance > rhs.attendance; }
  bool operator<(const User &rhs) const { return attendance < rhs.attendance; }
  bool operator==(const User &rhs) const {
    return attendance == rhs.attendance && id == rhs.id;
  }

  friend std::istream &operator>>(std::istream &is, User &user) {
    int id, attendance;
    is >> id >> attendance;
    user = {id, attendance};
    return is;
  }

  friend std::ostream &operator<<(std::ostream &os, const User &user) {
    return os << user.id;
  }
};

// template <> struct rc::Arbitrary<User> {
//   static rc::Gen<User> arbitrary() {
//     return rc::gen::build<User>(
//         rc::gen::set(&User::id, rc::gen::arbitrary<int>()),
//         rc::gen::set(&User::attendance, rc::gen::arbitrary<int>()));
//   }
// };

template <typename T>
std::vector<T> get_k_min_elements(T *data, std::size_t n, std::size_t k) {
  if (k == 0)
    return {};

  Heap<T, std::greater<>> heap;
  heap.grow(k + 1);

  for (std::size_t i = 0; i < k; i++)
    heap.template insert(data[i]);

  for (std::size_t i = k; i < n; i++) {
    auto max = heap.top();
    if (max > data[i]) {
      heap.pop();
      heap.insert(data[i]);
    }
    assert(heap.size() <= k);
  }

  Heap<T, std::less<>> copy{std::move(heap)};
  std::vector<T> result{k};
  for (std::size_t i = 0; i < k; i++) {
    result[i] = copy.top();
    copy.pop();
  }
  return result;
}

// void test() {
//   rc::check([] {
//     auto n = *rc::gen::inRange(0, 1'000);
//     auto k = *rc::gen::inRange(0, n);
//     auto vec = *rc::gen::uniqueBy<std::vector<User>>(
//         n, rc::gen::arbitrary<User>(),
//         [](const User &u) { return u.attendance; });
//
//     std::vector<User> sorted = vec;
//     std::sort(sorted.begin(), sorted.end(), std::less<>());
//     sorted = {sorted.begin(), sorted.begin() + k};
//
//     auto calc = get_k_min_elements(vec.data(), n, k);
//
//     for (int i = 0; i < k; i++)
//       RC_ASSERT(sorted[i] == calc[i]);
//   });
// }

void solve() {
  int n, k;
  std::cin >> n >> k;

  User *users = new User[n];
  for (int i = 0; i < n; i++)
    std::cin >> users[i];

  auto result = get_k_min_elements(users, n, k);
  for (int i = 0; i < k; i++)
    std::cout << result[i].id << ' ';

  delete[] users;
}

int main() { solve(); }