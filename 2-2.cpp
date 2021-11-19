/*
 * Дан массив целых чисел А[0..n-1].
 * Известно, что на интервале [0, m] значения массива строго возрастают, а на интервале [m, n-1] строго убывают.
 * Найти m за O( log m ).
 * 2 ≤ n ≤ 10000.
 */

#include <cassert>
#include <iostream>

template <typename T>
auto binary_search(T *array, std::size_t from, std::size_t to) {
  auto left = from;
  auto right = to;

  while (true) {
    auto mid = (right + left) / 2;
    if (array[mid] > array[mid + 1] && array[mid] > array[mid - 1])
      return mid;

    if (array[mid] > array[mid + 1])
      right = mid;
    else
      left = mid;
  }
}

template <typename T>
std::size_t find_extremum(T *array, std::size_t size) {
  assert(size >= 2);
  if (array[0] > array[1])
    return 0;
  else if (array[size - 1] > array[size - 2])
    return size - 1;

  std::size_t from = 1, to = 2;
  while(to < size && array[to] > array[from]) {
    from = to;
    to <<= 1;
  }
  return binary_search(array, from, std::min(to, size));
}

int main() {
  std::size_t n;
  std::cin >> n;

  int *array = new int[n];
  for (std::size_t i = 0; i < n; i++)
    std::cin >> array[i];

  std::cout << find_extremum(array, n) << std::endl;
  delete[] array;
}