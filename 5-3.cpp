/*
 * На числовой прямой окрасили N отрезков.
 * Известны координаты левого и правого концов каждого отрезка [Li, Ri]. Найти
 * длину окрашенной части числовой прямой.
 * N ≤ 10000. Li, Ri — целые числа в диапазоне [0, 10^9].
 */

#include <cassert>
#include <iostream>

#ifdef TEST
void test();
#endif

template <typename T, typename F = std::less<T>>
void merge(T *to, T *result, std::size_t left, std::size_t mid,
           std::size_t right, F compare = F()) {
  std::size_t i = 0, j = 0;

  while (left + i < mid && mid + j < right) {
    if (compare(to[left + i], to[mid + j])) {
      result[i + j] = std::move(to[left + i]);
      i++;
    } else {
      result[i + j] = std::move(to[mid + j]);
      j++;
    }
  }

  while (left + i < mid) {
    result[i + j] = std::move(to[left + i]);
    i++;
  }

  while (mid + j < right) {
    result[i + j] = std::move(to[mid + j]);
    j++;
  }

  std::swap_ranges(result, result + i + j, to + left);
}

template <typename T, typename F = std::less<T>>
void merge_sort(T *array, std::size_t size, F compare = F()) {
  T *buffer = new T[size];
  for (std::size_t i = 1; i < size; i <<= 1)
    for (std::size_t j = 0; j < size - i; j += 2 * i)
      merge(array, buffer, j, j + i, std::min(j + 2 * i, size), compare);
  delete[] buffer;
}

enum class PointType {
  Starting,
  Ending
};

struct Point {
  int x;
  PointType type;

  bool operator<(const Point &rhs) const { return x < rhs.x; }
};

std::size_t get_max_length(Point *points, std::size_t size) {
  std::size_t depth = 0, length = 0;
  for (std::size_t i = 0; i < size - 1; i++) {
    if (points[i].type == PointType::Starting)
      depth++;
    else
      depth--;
    if (depth > 0)
      length += points[i + 1].x - points[i].x;
  }
  return length;
}

int main() {
#ifdef TEST
  test();
#else
  int n;
  std::cin >> n;

  auto *points = new Point[2 * n];
  for (int i = 0; i < n; i++) {
    int s, e;
    std::cin >> s >> e;
    points[2 * i] = Point{s, PointType::Starting};
    points[2 * i + 1] = Point{e, PointType::Ending};
  }

  merge_sort(points, 2 * n);
  std::cout << get_max_length(points, 2 * n);
  delete[] points;
#endif
}

#ifdef TEST

#include <rapidcheck.h>

namespace rc {
template <>
struct Arbitrary<Point> {
  static Gen<Point> arbitrary() {
    return gen::build<Point>(gen::set(&Point::x, gen::arbitrary<int>()),
                             gen::set(&Point::type, gen::element(PointType::Starting, PointType::Ending)));
  }
};
}// namespace rc

void test() {
  rc::check([](std::vector<Point> points) {
    RC_PRE(points.size() > 0);
    std::vector<Point> copy = points;
    std::sort(copy.begin(), copy.end());

    merge_sort(points.data(), points.size());
    RC_ASSERT(get_max_length(copy.data(), copy.size()) == get_max_length(points.data(), points.size()));
  });
}

#endif