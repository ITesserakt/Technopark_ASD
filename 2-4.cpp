/*
 * Дан отсортированный массив различных целых чисел A[0..n-1] и массив целых
 * чисел B[0..m-1]. Для каждого элемента массива B[i] найдите минимальный индекс
 * элемента массива A[k], ближайшего по значению к B[i]. n ≤ 110000, m ≤ 1000.
 * Время работы поиска для каждого элемента B[i]: O(log(k)).
 */

#include <iostream>

template <typename T>
auto binary_search(T *array, T element, std::size_t size, std::size_t from, std::size_t to) {
  auto left = from;
  auto right = to;

  while (right - left > 1) {
    auto mid = (right + left) / 2;

    if (array[mid] > element)
      right = mid;
    else
      left = mid;
  }
  return left;
}

template <typename T> int find_max_below(T *array, int length, T below) {
  int range = 1;
  while (range < length && array[range] < below)
    range <<= 1;

  int found =
      binary_search(array, below, length, 0, length);

  if (found == length - 1 || found == 0)
    return found;

  auto diff = std::abs(array[found] - below);
  if (diff == 0) {
    return found;
  } else if (std::abs(array[found + 1] - below) < diff) {
    return found + 1;
  } else if (std::abs(array[found - 1] - below) < diff) {
    return found - 1;
  }
  return found;
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