#include <stdio.h>
#include "test.hpp"
#define UNIONFIND_IMPLEMENTATION
#include "../src/unionfind.hpp"

int unionfind_test() {

  SETUP("UNIONFIND");

  auto uf = unionfindinit(16);

  // TODO: do some tests

  EQUALS(uf.non_destructive_find(4), 4, "find(4) = 4");
  EQUALS(uf._union(1, 2), true, "union(1, 2) = true");
  EQUALS(uf.non_destructive_find(1), uf.non_destructive_find(2), "find(1) = find(2)");

  // after this 4, 5, 6, 7 and 8 should be in the same partition
  uf._union(4, 5);
  uf._union(7, 8);
  uf._union(6, 7);
  uf._union(5, 6);

  // 8 should be the representative element of the partition, confirming that
  EQUALS(uf.non_destructive_find(4), 8, "find(4) = 8");
  EQUALS(uf.non_destructive_find(5), 8, "find(5) = 8");
  EQUALS(uf.non_destructive_find(6), 8, "find(6) = 8");
  EQUALS(uf.non_destructive_find(7), 8, "find(7) = 8");
  EQUALS(uf.non_destructive_find(8), 8, "find(8) = 8");

  unionfindfree(&uf);

  DONE();

  return 0;
}
