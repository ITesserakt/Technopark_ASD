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

  std::size_t max =
      first >= second ? std::max(first, third) : std::max(second, third);
  std::size_t min =
      first <= second ? std::min(first, third) : std::min(second, third);

  return first + second + third - max - min;
}

std::size_t random_pivot(std::size_t from, std::size_t to) {
  std::uniform_int_distribution<std::size_t> distribution{from, to};
  return distribution(engine);
}

std::size_t first_pivot(std::size_t from, std::size_t to) { return from; }
std::size_t last_pivot(std::size_t from, std::size_t to) { return to; }

template <typename T, typename P, typename C>
std::size_t partition_backward(T *array, std::size_t from, std::size_t to,
                               P pivot_fn, C comparator) {
  auto pivot = pivot_fn(from, to);
  std::swap(array[pivot], array[from]);
  pivot = from;
  auto i = to;
  auto j = to;

  while (pivot < j) {
    if (comparator(array[j], array[pivot]))
      std::swap(array[i--], array[j]);
    j--;
  }
  std::swap(array[i], array[pivot]);
  return i;
}

template <typename T, typename P, typename C>
std::size_t partition_forward(T *array, std::size_t from, std::size_t to,
                              P pivot_fn, C comparator) {
  auto pivot = pivot_fn(from, to);
  std::swap(array[pivot], array[to]);
  pivot = to;
  auto i = from;
  auto j = from;

  while (j < pivot) {
    if (comparator(array[j], array[pivot]))
      std::swap(array[i++], array[j]);
    j++;
  }
  std::swap(array[i], array[pivot]);
  return i;
}

template <typename T, typename F = std::greater<T>>
T k_order_statistics(T *array, std::size_t size, std::size_t k,
                     F compare = F()) {
  std::size_t left = 0;
  auto right = size - 1;
  while (true) {
    std::size_t mid =
        partition_backward(array, left, right, median_of_three, compare);

    if (mid == k)
      return array[mid];
    else if (k < mid)
      right = mid - 1;
    else
      left = mid + 1;
  }
}

template <typename T, typename F = std::greater<T>>
double nth_percentile(T *array, std::size_t n, std::size_t p, F compare = F()) {
  std::size_t index = n * p / 100;
  return k_order_statistics(array, n, index, compare);
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