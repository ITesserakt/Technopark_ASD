/*
 * Дано множество целых чисел из [0..109] размера n.
 * Используя алгоритм поиска k-ой порядковой статистики, требуется найти
 * следующие параметры множества:
 * 10% перцентиль
 * медиана (50% перцентиль)
 * 90% перцентиль
 */

#include <iostream>

template <typename T, typename F = std::less<T>>
std::size_t partition(T *array, std::size_t from, std::size_t to,
                      F compare = std::less<T>()) {
  T mid = array[(from + to) / 2];
  while (from <= to) {
    while (compare(array[from], mid))
      from++;
    while (!compare(array[to], mid) && array[to] != mid)
      to--;
    if (from >= to)
      break;
    std::swap(array[from++], array[to--]);
  }
  return to;
}

template <typename T, typename F = std::less<T>>
T k_order_statistics(T *array, std::size_t size, std::size_t k,
                     F compare = std::less<T>()) {
  std::size_t left = 0;
  auto right = size - 1;
  while (true) {
    std::size_t mid = partition(array, left, right, compare);

    if (mid == k)
      return array[mid];
    else if (k < mid)
      right = mid;
    else
      left = mid + 1;
  }
}

template <typename T>
double nth_percentile(T *array, std::size_t n, std::size_t p) {
  std::size_t index = n * p / 100;
  return k_order_statistics(array, n, index);
}

int main() {
  int n;
  std::cin >> n;
  if (n == 0)
    return 0;
  int *array = new int[n];

  for (int i = 0; i < n; i++)
    std::cin >> array[i];

  std::cout << nth_percentile(array, n, 10) << std::endl
            << nth_percentile(array, n, 50) << std::endl
            << nth_percentile(array, n, 90);

  delete[] array;
}