#include <stdio.h>
#include "test.hpp"
#define UNIONFIND_IMPLEMENTATION
#include "../src/unionfind.hpp"

int unionfind_test() {

  SETUP("UNIONFIND");

  auto uf = unionfindinit(16);

  // TODO: do some tests

  EQUALS(uf.find(4), 4, "TODO");

  unionfindfree(&uf);

  DONE();

  return 0;
}
