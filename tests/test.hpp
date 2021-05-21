#pragma once

#define RUNNER() { \
  if(0) { \
exit_suite: \
    return 1; \
  } \
}

#define SUITE(a) { \
  if(a != 0) { goto exit_suite; } \
}

#define SETUP(a) \
  int test_cnt = 0; \
  char* name = (a); \
  printf("%s TEST\n", name); \
  if(0) { \
exit: \
    printf("[%s] exited after %d tests", name, test_cnt); \
    printf("----------\n\n"); \
    return 1; \
  }

#define EQUALS(a, b, c) { \
    typeof(a) _a = (a); \
    typeof(b) _b = (b); \
    if(_a == _b) { \
      printf("[%d] test successful: %s\n", test_cnt++, c); \
    } else { \
      printf("[%d] test failed: expected \"%d\" but got \"%d\" (%s)\n", test_cnt++, _b, _a, c); \
      goto exit; \
    } \
  }

#define DONE() { \
    printf("[%s] successfully ran %d tests\n", name, test_cnt); \
    printf("----------\n\n"); \
  }
