#include <cstdlib>
#include "common.hpp"

template<class T> struct minheap {
  size_t m_len;
  size_t m_cap;
  T* m_data;

  void resize(size_t new_size);
  void dbg();
  void dbg(char* name);

  void insert(T el);
  void sift_up(size_t i);
  void sift_down(size_t i);
  T min();
  T delete_min();
};

template<class T> static minheap<T> minheapinit(size_t capacity);
template<class T> static void minheapfree(minheap<T>* heap);

#ifdef MINHEAP_IMPLEMENTATION

#define PARENT(n) (n/2)
#define L_CHILD(n) (2*n+1)
#define R_CHILD(n) (2*n+2)
#define L_OR_R(n) ((n+1) % 2)
#define SIBLING(n) (L_OR_R(n) ? ((n)-1) : ((n)+1))

#define SWAP(a, b) { \
  __typeof__ (a) tmp = a; \
  a = b; \
  b = tmp; \
}



/**
 * Create a new minheap using an initial capacity.
 */
template<class T> static minheap<T> minheapinit(size_t capacity) {
  size_t actual_capacity = MAX(capacity, 3);

  T* data = (T*)(malloc(sizeof(T) * actual_capacity));
  // TODO: handle malloc errors

  for(size_t i = 0; i < actual_capacity; i++) { data[i] = 0; }

  minheap<T> heap = {
    .m_len = 0,
    .m_cap = actual_capacity,
    .m_data = data
  };

  return heap;
}

/**
 * Free minheap
 */
template<class T> static void minheapfree(minheap<T>* heap) {
  heap->m_len = 0;
  heap->m_cap = 0;
  free(heap->m_data);
}

/**
 * Resize the minheap
 *
 * This can be called manually but will also be called internally whenever needed.
 * It might be useful to call this if you already know that you'll add a specific amount of items soon.
 */
template<class T> void minheap<T>::resize(size_t new_size) {
  if(new_size >= m_cap) {
    m_cap = m_cap * 2;
    m_data = (T*)(realloc(m_data, sizeof(T) * m_cap));
  } else if(new_size <= m_cap / 4) {
    m_cap = m_cap / 2;
    m_data = (T*)(realloc(m_data, sizeof(T) * m_cap));
  }
}

/**
 * Print debug information about the state of the minheap
 */
template<class T> void minheap<T>::dbg() {
  dbg(NULL);
}

/**
 * Print debug infomration about the state of the minheap
 *
 * Includes a name which will be displayed alongside the debug information to tell debug logs apart.
 */
template<class T> void minheap<T>::dbg(char* name) {
  if(name != NULL) { printf("dbg (%s):\n ", name); }
  else             { printf("dbg:\n");             }

  printf("- m_len: %d\n", m_len);
  printf("- m_cap: %d\n", m_cap);
  printf("- m_dat: (raw) [ ");
  for(size_t i = 0; i < m_len - 1; i++) { printf("%2d, ", m_data[i]); }
  printf("%2d ]\n", m_data[m_len-1]);
  printf("- m_dat: (int) [\n");
  for(size_t i = 0; i < m_len; i++) {
    printf(
      "  (%2d) { .value = %2d, .lchild = %2d, rchild = %2d }\n",
      i,
      m_data[i],
      L_CHILD(i) < m_len ? m_data[L_CHILD(i)] : -1,
      R_CHILD(i) < m_len ? m_data[R_CHILD(i)] : -1
    );
  }
  printf("  ]\n");
}

/**
 * Insert something into the minheap
 *
 * O(log n)
 */
template<class T> void minheap<T>::insert(T el) {
  resize(m_len+1);

  m_data[m_len] = el;
  sift_up(m_len++);
}


/**
 * Correct minheap invariant from the bottom up
 *
 * Mostly used internally.
 * It is assumed that the minheap invariant holds true everywhere except at m_data[i]
 */
template<class T> void minheap<T>::sift_up(size_t i) {

  // propagate the element at position i up as far as it needs to by comparing
  // it with it's parent element until the element is at the correct position

  if(i == 0 || m_data[PARENT(i)] <= m_data[i]) return;

  SWAP(m_data[i], m_data[PARENT(i)]);
  sift_up(PARENT(i));
}


/**
 * Correct minheap invariant from the top down
 *
 * Mostly used internally.
 * It is assumed that the minheap invariant holds true everywhere except at L_CHILD(i) and R_CHILD(i).
 */
template<class T> void minheap<T>::sift_down(size_t i) {

  // the invariant is possibly broken for L_CHILD(i) and R_CHILD(i).
  // this is being fixed by finding the "target child" (m) and then swapping
  // i with it. the target child is the smaller child (also need to check if
  // the right child even exists; if not choose left child as well).
  // after this the invariant might be broken for L_CHILD(m) and R_CHILD(m)
  // => repeat until the end has been reached (meaning going out of bounds).

  if(L_CHILD(i) <= m_len) {
    size_t m = (R_CHILD(i) >= m_len || m_data[L_CHILD(i)] <= m_data[R_CHILD(i)])
      ? L_CHILD(i)
      : R_CHILD(i);

    if(m_data[i] > m_data[m]) {
      SWAP(m_data[i], m_data[m]);
      sift_down(m);
    }
  }
}

/**
 * Get the minimum element of the minheap
 *
 * O(1)
 */
template<class T> T minheap<T>::min() {
  return m_len == 0 ? NULL : m_data[0];
}


/**
 * Delete minimum element of minheap and return it
 *
 * O(log n)
 */
template<class T> T minheap<T>::delete_min() {
  T result = m_data[0];
  m_data[0] = m_data[--m_len];
  sift_down(0);

  resize(m_len-1);
  return result;
}

#endif
