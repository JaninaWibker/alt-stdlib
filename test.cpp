#include <stdio.h>
#include "tests/test.hpp"
#include "tests/array.cpp"
#include "tests/hashtable.cpp"
#include "tests/hashmap.cpp"

int main() {

  RUNNER();

  SUITE(array_test());
  SUITE(hashtable_test());
  SUITE(hashmap_test());
}
