#include <stdio.h>
#include "test.hpp"
#define MINHEAP_IMPLEMENTATION
#include "../src/minheap.hpp"

int minheap_test() {

  SETUP("MINHEAP");

  minheap<int> heap = minheapinit<int>(15);

  heap.insert(5);
  heap.insert(4);
  heap.insert(13);

  heap.dbg();

  minheapfree(&heap);

  DONE();

  return 0;
}
