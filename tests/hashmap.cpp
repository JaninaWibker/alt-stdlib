#include <stdio.h>
#include "../src/hashmap.hpp"

int hashmap_test() {

  auto hash = [](char* key) -> int { return key[0] % 4; };

  auto hm = hashmapinit<char*, int, decltype(hash)>(4, hash);

  hm.ins("a", 42);
  hm.ins("b", 43);

  // hash clashes with "a"; this means that a free slot is going to be used instead
  hm.ins("e", 2);

  // hash clashes with "a" and "e"
  hm.ins("i", 22);

  // hash doesn't clash but this slot is already in use
  hm.ins("c", 21);

  hm.del("e");

  hm.fnd((char*)("c"));

  hm.each([](char* key, int value) { printf("%s: %d\n", key, value); });

  hashmapfree(&hm);

  return 0;
}

