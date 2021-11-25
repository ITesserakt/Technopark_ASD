/*
 * Дано множество целых чисел из [0..109] размера n.
 * Используя алгоритм поиска k-ой порядковой статистики, требуется найти
 * следующие параметры множества:
 * 10% перцентиль
 * медиана (50% перцентиль)
 * 90% перцентиль
 */

#include <iostream>
#include <random>

std::default_random_engine engine{std::random_device{}()};

std::size_t median_of_three(std::size_t from, std::size_t to) {
  std::uniform_int_distribution<std::size_t> distribution{from, to};
  auto first = distribution(engine);
  auto second = distribution(engine);
  auto third = distribution(engine);
  return first + second + third - std::max({first, second, third}) -
         std::min({first, second, third});
}

std::size_t random_pivot(std::size_t from, std::size_t to) {
  std::uniform_int_distribution<std::size_t> distribution{from, to};
  return distribution(engine);
}

template <typename T, typename G, typename F = std::greater<T>>
std::size_t partition_forward(T *array, std::size_t from, std::size_t to,
                              G pivot_fn, F compare = std::greater<T>()) {
  std::size_t pivot = to, i = from;
  std::swap(array[pivot_fn(from, to)], array[to]);
  for (std::size_t j = from; j < to; j++) {
    if (!compare(array[j], array[pivot])) {
      std::swap(array[i], array[j]);
      i++;
    }
  }
  std::swap(array[i], array[pivot]);
  return i;
}

template <typename T, typename F = std::greater<T>>
T k_order_statistics(T *array, std::size_t size, std::size_t k,
                     F compare = std::greater<T>()) {
  std::size_t left = 0;
  auto right = size - 1;
  while (true) {
    std::size_t mid =
        partition_forward(array, left, right, random_pivot, compare);

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