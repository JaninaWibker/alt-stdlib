#include <stdio.h>
#include "src/array.hpp"

int main() {
  array<int> arr = arrayinit<int>(0);

  arr.put(0, 7);
  arr.put(1, 8);
  arr.put(2, 9);
  arr.put(3, 11);
  arr.del(3);
  arr.put(3, 10);

  printf("initial\n");

  arr.each([](int x, int i) { printf("data[%d] = %d\n", i, x); });

  array<int> arr2 = arrayinit<int>(2);
  arr2.push(11);
  arr2.push(12);

  arr.cat(arr2);

  printf("concat\n");

  arr.each([](int x, int i) { printf("data[%d] = %d\n", i, x); });

  arr.filter([](int x, int i) { return x > 8; });

  printf("filter\n");

  arr.each([](int x, int i) { printf("data[%d] = %d\n", i, x); });

  printf("len: %d, cap: %d\n", arr.len(), arr.cap());

  arr.fill('a');

  arr.each([](int x, int i) { printf("%c", x); });

  printf("\n");
}
