#include <stdio.h>
#include "test.hpp"
#define HASHMAP_IMPLEMENTATION
#include "../src/hashmap.hpp"

int hashmap_test() {

  SETUP("HASHMAP");

  auto hash = [](char* key) -> int { return key[0] % 4; };

  auto hm = hashmapinit<char*, int, decltype(hash)>(4, hash);

  EQUALS(hm.ins("a", 42), 42, "set 'a' to 42");
  EQUALS(hm.ins("b", 43), 43, "set 'b' to 43");

  // hash clashes with "a"; this means that a free slot is going to be used instead
  EQUALS(hm.ins("e", 2),  2, "set 'e' to  2 (hash clashes with 'a')");

  // hash clashes with "a" and "e"
  EQUALS(hm.ins("i", 22), 22, "set 'i' to 22 (hash clashes with 'a' & 'e')");

  // hash doesn't clash but this slot is already in use
  EQUALS(hm.ins("c", 21), 21, "set 'c' to 21 (slot already in use)");

  EQUALS(hm.del("e"), 2, "deleted key 'e' (had value 2)");

  EQUALS(hm.fnd((char*)("c")), 21, "searched for key 'c'");

  hm.each([](char* key, int value) { printf("%s: %d\n", key, value); });

  // TODO: write tests which test other functions

  hashmapfree(&hm);

  DONE();

  return 0;
}

