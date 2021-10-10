/*
 *  Подсчитать кол-во единичных бит во входном числе, стоящих на четных позициях.
 *  Позиции битов нумеруются с 0.
 *
 *  Необходимо использование битовых операций.
 *  Использование арифметических операций запрещено.
 */

#include <iostream>

int count_even_ones(unsigned int number) {
  int counter = 0;
  bool isEven = true;

  while (number > 0) {
    // проверяем, что бит четный и что он равен 1
    if (isEven && (number & 1) == 1)
      ++counter;
    isEven = !isEven;
    number >>= 1;
  }

  return counter;
}

int main() {
  unsigned int number;
  std::cin >> number;

  std::cout << count_even_ones(number) << std::endl;
}
