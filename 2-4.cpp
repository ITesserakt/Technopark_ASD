/*
 * Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых
 * чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
 * элемента массива A[k], ближайшего по значению к B[i]. n ≤ 110000, m ≤ 1000.
 * Время работы поиска для каждого элемента B[i]: O(log(k)).
 */

#include <iostream>

template <typename T>
auto binary_search(T *array, T element, std::size_t from, std::size_t to) {
  auto left = from;
  auto right = to;

  while (right - left > 1) {
    auto mid = (right + left) / 2;

    if (array[mid] > element)
      right = mid;
    else
      left = mid;
  }
  return std::make_pair(left, right);
}

template <typename T>
std::size_t find_max_below(T *array, std::size_t length, T below) {
  if (below <= array[0])
    return 0;
  if (array[length - 1] <= below)
    return length - 1;

  std::size_t range = 1;
  while (range < length && array[range] < below)
    range <<= 1;
  if (range > length - 1)
    range = length - 1;

  auto [left, right] = binary_search(array, below, range / 2, range);

  if (std::abs(array[left] - below) <= std::abs(array[right] - below))
    return left;
  return right;
}

int main() {
  std::size_t n, m;

  std::cin >> n;
  int *a = new int[n];
  for (std::size_t i = 0; i < n; i++)
    std::cin >> a[i];

  std::cin >> m;
  for (std::size_t i = 0; i < m; i++) {
    int next;
    std::cin >> next;
    std::cout << find_max_below(a, n, next) << ' ';
  }

  delete[] a;
}