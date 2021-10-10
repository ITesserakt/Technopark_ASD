/*
 * Если в числе содержится только один бит со значением 1, записать в выходной поток OK.
 * Иначе записать FAIL.
 * Необходимо использование битовых операций.
 * Использование арифметических операций запрещено.
 */

#include <iostream>

int count_ones(unsigned int number) {
  int counter = 0;

  while (number > 0) {
    if ((number & 1) == 1)
      counter++;
    number >>= 1;
  }

  return counter;
}

int main() {
  unsigned int n;
  std::cin >> n;

  if (count_ones(n) == 1)
    std::cout << "OK" << std::endl;
  else
    std::cout << "FAIL" << std::endl;
}
