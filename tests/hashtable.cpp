#include <stdio.h>
#include "../src/hashtable.hpp"

int hashtable_test() {

  auto hash = [](int x) -> int { return x % 4; };

  auto ht = hashtableinit<int, decltype(hash)>(4, hash);

  ht.ins(42);
  ht.ins(43);

  // hash clashes with 42; this means that another free slot is going to be used instead
  ht.ins(2);

  // hash clashes with 42 and 2
  ht.ins(22);

  // hash doesn't clash but this slot is already in use
  ht.ins(21);

  ht.del(2);

  ht.each([](int x) { printf("%d\n", x); });

  return 0;
}
