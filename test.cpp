#include <stdio.h>
#include "tests/array.cpp"
#include "tests/hashtable.cpp"
#include "tests/hashmap.cpp"

int main() {
  printf("ARRAY TEST\n");
  array_test();
  printf("\n----------\n");

  printf("HASHTABLE TEST\n");
  hashtable_test();
  printf("\n----------\n");

  printf("HASHMAP TEST\n");
  hashmap_test();
  printf("\n----------\n");
}
