#include <stdio.h>
#include "test.hpp"
#define HASHTABLE_IMPLEMENTATION
#include "../src/hashtable.hpp"

int hashtable_test() {

  SETUP("HASHTABLE");

  auto hash = [](int x) -> int { return x % 4; };

  auto ht = hashtableinit<int, decltype(hash)>(4, hash);

  EQUALS(ht.ins(42), 42, "inserted 42 (first of it's kind; no clashes)");
  EQUALS(ht.ins(43), 43, "inserted 43 (first of it's kind; no clashes)");

  // hash clashes with 42; this means that a free slot is going to be used instead
  EQUALS(ht.ins( 2),  2, "inserted  2 (clashes with 42)");

  // hash clashes with 42 and 2
  EQUALS(ht.ins(22), 22, "inserted 22 (clashes with 42 & 2)");

  // hash doesn't clash but this slot is already in use
  EQUALS(ht.ins(21), 21, "inserted 21 (doesn't clash but slot is already taken)");

  EQUALS(ht.del( 2), true, "deleted 2");

  EQUALS(ht.fnd(21), 21, "deleted 21");

  ht.each([](int x) { printf("%d\n", x); });

  // TODO: write tests which test other functions

  hashtablefree(&ht);

  DONE();

  return 0;
}
