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

template <typename T, typename Comparator = std::less<T>>
class Heap {
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

  template <typename U>
  void insert(U &&value) {
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
};

template <typename T, typename Proj>
struct ProjectionLess {
  Proj f;

  bool operator()(T &a, T &b) { return f(a) < f(b); }
};

template <typename T, typename PriorityProj>
struct PriorityQueue : Heap<T, ProjectionLess<T, PriorityProj>> {
  explicit PriorityQueue(PriorityProj proj)
      : Heap<T, ProjectionLess<T, PriorityProj>>({proj}) {}
};

struct Process {
public:
  unsigned int priority;
  unsigned int time_worked;
  unsigned int total;

  friend std::istream &operator>>(std::istream &is, Process &p) {
    unsigned int priority, total;
    is >> priority >> total;
    p = {priority, 0, total};
    return is;
  }
};

class Processor {
private:
  int context_toggled = 0;
  PriorityQueue<Process, unsigned int (*)(Process &)> queue{
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

#ifdef TEST
void test();
#endif

int main() {
#ifdef TEST
  test();
#else
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
#endif
}

#ifdef TEST
#include <rapidcheck.h>

template <>
struct rc::Arbitrary<Process> {
  static rc::Gen<Process> arbitrary() {
    return rc::gen::build<Process>(rc::gen::set(&Process::priority, rc::gen::inRange(1, 1'000'000)),
                                   rc::gen::set(&Process::time_worked, rc::gen::just(0)),
                                   rc::gen::set(&Process::total, rc::gen::inRange(1, 1'000'000)));
  }
};

void test() {
  rc::check([](std::deque<Process> vec) {
    RC_PRE(!vec.empty());
    Processor processor;
    for (auto process : std::vector<Process>{vec.begin(), vec.end()})
      processor.addProcess(process);
    processor.run();

    ProjectionLess<Process, unsigned int (*)(Process)> compare = {[](Process p) { return p.priority * (p.time_worked + 1); }};
    std::make_heap(vec.begin(), vec.end(), compare);
    int switched = 0;
    while (!vec.empty()) {
      auto next = vec.front();
      vec.pop_front();

      if (next.time_worked < next.total) {
        switched++;
        next.time_worked += next.priority;
        vec.push_back(next);
      }
    }
    RC_ASSERT(switched == processor.switched());
  });
}
#endif