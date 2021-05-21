#include <stdio.h>
#include "test.hpp"
#include "../src/array.hpp"

int array_test() {

  SETUP("ARRAY");

  array<int> arr = arrayinit<int>(0);

  EQUALS(arr.put(0,  7),  7, "arr[0] =  7");
  EQUALS(arr.put(1,  8),  8, "arr[1] =  8");
  EQUALS(arr.put(2,  9),  9, "arr[2] =  9");
  EQUALS(arr.put(3, 11), 11, "arr[3] = 11");
  EQUALS(arr.del(3), 11, "delete arr[3]");
  EQUALS(arr.put(3, 10), 10, "arr[3] = 10");

  printf("initial\n");

  arr.each([](int x, int i) { printf("data[%d] = %d\n", i, x); });

  array<int> arr2 = arrayinit<int>(2);

  EQUALS(arr2.push(11), 11, "arr2.push(11)");
  EQUALS(arr2.push(12), 12, "arr2.push(12)");

  arr.cat(arr2);

  // TODO: write tests that integrate with each and test more functions other than just .put and .del

  printf("concat\n");

  arr.each([](int x, int i) { printf("data[%d] = %d\n", i, x); });

  arr.filter([](int x, int i) { return x > 8; });

  printf("filter\n");

  arr.each([](int x, int i) { printf("data[%d] = %d\n", i, x); });

  printf("len: %d, cap: %d\n", arr.len(), arr.cap());

  arr.fill('a');

  arr.each([](int x, int i) { printf("%c", x); });
  printf("\n");

  DONE();

  return 0;
}
