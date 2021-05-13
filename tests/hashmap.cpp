#include <stdio.h>
#include "../src/hashmap.hpp"

int hashmap_test() {

  auto hash = [](char* key) -> int { return key[0] % 4; };

  auto ht = hashmapinit<char*, int, decltype(hash)>(4, hash);

  // TODO: do stuff

  // ht.each([](char* key, int value) { printf("%s: %d\n", key, value); });

  hashmapfree(&ht);

  return 0;
}
