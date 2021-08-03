#include <stdio.h>

#define TEST_IMPLEMENTATION

#include "tests/test.hpp"
#include "tests/array.cpp"
#include "tests/hashtable.cpp"
#include "tests/hashmap.cpp"
#include "tests/minheap.cpp"

int main() {

  RUNNER();

  SUITE(array_test());
  SUITE(hashtable_test());
  SUITE(hashmap_test());
  SUITE(minheap_test());
}
