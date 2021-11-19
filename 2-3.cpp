/*
 * Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию.
 * A[0..n-1] и B[0..m-1]. n » m.
 * Найдите их пересечение.
 * Требуемое время работы: O(m * log k), где k - позиция элементта B[m-1] в массиве A..
 * В процессе поиска очередного элемента B[i] в массиве A пользуйтесь результатом поиска элемента B[i-1]. n, k ≤ 10000.
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
  return left;
}

template <typename T>
void print_intersection(T *array, std::size_t array_size, T *other, std::size_t other_size) {
  std::size_t last_position = 0;
  for (std::size_t i = 0; i < other_size; i++) {
    auto found = binary_search(array, other[i], last_position, array_size);
    if (found == array_size || array[found] != other[i])
      continue;

    std::cout << other[i] << ' ';
    last_position = found;
  }
}

int main() {
  std::size_t n, m;
  std::cin >> n >> m;

  int *a = new int[n];
  int *b = new int[m];

  for (std::size_t i = 0; i < n; i++)
    std::cin >> a[i];
  for (std::size_t i = 0; i < m; i++)
    std::cin >> b[i];

  print_intersection(a, n, b, m);
}