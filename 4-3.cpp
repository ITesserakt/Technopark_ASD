/*
 * В операционной системе Technux есть планировщик процессов.
 * Каждый процесс характеризуется:
 * приоритетом P
 * временем, которое он уже отработал t
 * временем, которое необходимо для завершения работы процесса T
 *
 * Планировщик процессов выбирает процесс с минимальным значением P * (t + 1),
 * выполняет его время P и кладет обратно в очередь процессов. Если выполняется
 * условие t >= T, то процесс считается завершенным и удаляется из очереди.
 * Требуется посчитать кол-во переключений процессора.
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

template <typename T, typename Proj> struct ProjectionLess {
  Proj f;

  bool operator()(T &a, T &b) { return f(a) < f(b); }
};

template <typename T, typename PriorityProj>
struct PriorityQueue : Heap<T, ProjectionLess<T, PriorityProj>> {
  explicit PriorityQueue(PriorityProj proj)
      : Heap<T, ProjectionLess<T, PriorityProj>>(
            ProjectionLess<T, PriorityProj>{proj}) {}
};

struct Process {
public:
  int priority;
  int time_worked;
  int total;

  friend std::istream &operator>>(std::istream &is, Process &p) {
    int priority, total;
    is >> priority >> total;
    p = {priority, 0, total};
    return is;
  }
};

class Processor {
private:
  int context_toggled = 0;
  PriorityQueue<Process, int (*)(Process &)> queue{
      [](Process &p) { return p.priority * (p.time_worked + 1); }};

public:
  [[nodiscard]] int switched() const { return context_toggled; }

  void addProcess(Process p) { queue.insert(p); }

  void run() {
    while (queue.size() > 0) {
      auto next = queue.top();
      queue.pop();

      if (next.time_worked < next.total) {
        context_toggled++;
        next.time_worked += next.priority;
        queue.insert(next);
      }
    }
  }
};

int main() {
  int n;
  std::cin >> n;
  Processor processor;

  for (int i = 0; i < n; i++) {
    Process process;
    std::cin >> process;
    processor.addProcess(process);
  }

  processor.run();
  std::cout << processor.switched();
}