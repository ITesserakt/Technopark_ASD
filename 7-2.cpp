/*
 * Дан массив неотрицательных целых 64-битных чисел. Количество чисел не больше
 * 1000000.
 * Отсортировать массив методом поразрядной сортировки LSD по байтам.
 */

#include <iostream>

template <typename N>
unsigned char get_k_byte(N number, auto k) requires std::unsigned_integral<N> {
  return (number >> 8 * k) & 0xFF;
}

// требует O(n) памяти и работает за O(k * n), где k - число байт в типе N
template <typename N>
void lsd_sort(N *array, std::size_t size) requires std::unsigned_integral<N> {
  std::size_t bytes[256] = {0};
  auto *result = new N[size];
  for (std::size_t i = 0; i < sizeof(N); i++) {
    for (auto &bit : bytes)
      bit = 0;

    for (int j = 0; j < size; j++)
      bytes[get_k_byte(array[j], i)]++;

    std::size_t count = 0;
    for (auto &bit : bytes) {
      auto temp = bit;
      bit = count;
      count += temp;
    }

    for (int j = 0; j < size; j++) {
      auto digit = get_k_byte(array[j], i);
      result[bytes[digit]] = array[j];
      bytes[digit]++;
    }
    std::swap_ranges(result, result + size, array);
  }
  delete[] result;
}

int main() {
  int n;
  std::cin >> n;

  auto *array = new unsigned long long[n];
  for (int i = 0; i < n; i++)
    std::cin >> array[i];

  lsd_sort(array, n);

  for (int i = 0; i < n; i++)
    std::cout << array[i] << ' ';
  delete[] array;
}