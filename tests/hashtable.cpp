#include <stdio.h>
#include "../src/hashtable.hpp"

int hashtable_test() {

  auto hash = [](int x) -> int { return x % 4; };

  auto ht = hashtableinit<int, decltype(hash)>(4, hash);

  ht.ins(42);
  ht.ins(43);

  ht.each([](int x) { printf("%d\n", x); });

  return 0;
}
