/*
 * Вернуть значение бита в числе N по его номеру K.
 * Позиции битов нумеруются с 0.
 *
 * Необходимо использование битовых операций.
 * Использование арифметических операций запрещено.
 */

#include <iostream>

int get_k_bit(unsigned int number, int k) {
  unsigned int pos = 1 << k;
  return (number & pos) == pos;
}

int main() {
  unsigned int n;
  int k;
  std::cin >> n >> k;

  std::cout << get_k_bit(n, k) << std::endl;
}
