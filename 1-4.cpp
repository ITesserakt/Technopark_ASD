/*
 * Инвертируйте значение бита в числе N по его номеру K.
 *
 * Необходимо использование битовых операций.
 * Использование арифметических операций запрещено.
 */

#include <iostream>

unsigned int with_inverted_bit(unsigned int number, int k) {
  unsigned int pos = 1 << k;
  return number ^ pos;
}

int main() {
  unsigned int n;
  int k;
  std::cin >> n >> k;

  std::cout << with_inverted_bit(n, k) << std::endl;
}
