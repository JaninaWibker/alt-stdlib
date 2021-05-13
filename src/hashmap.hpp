#include <cstdlib>
#include "common.hpp"

// #define HM_DEBUG

// the slot is part of the free list
#define SLOT_STATUS_FREE 0x01
// the data is at the correct slot hash-wise (head of linked list)
#define SLOT_STATUS_HEAD 0x02

template<class K, class V>
struct hm_slot {
  size_t prev;
  size_t next;
  char status;
  K key;
  V value;
};

template<class K, class V, class H> struct hashmap {
  size_t m_cap;
  size_t m_cnt;
  size_t m_freelist;
  hm_slot<K, V>* m_data;
  H m_hash;

  size_t cap() { return m_cap; }
  size_t cnt() { return m_cnt; }

  void resize(size_t new_size);

  V ins(K key, V value);
  V del(K key);
  V fnd(K key);
  template<class F> V fnd(F&& pred);
  bool has(K key);
  template<class F> bool has(F&& pred);
  template<class F> void each(F&& fn);
  void clr();
  template<class F> void filter(F&& pred);
  void cat(hashmap<K, V, H> other);

  void dbg();
  void dbg(char* name);
};

template<class K, class V, class H> hashmap<K, V, H> hashmapinit(size_t capacity, H hash) {

  // minimum capacity is 8 // TODO: should i change this to 16?
  size_t actual_capacity = max(capacity, 8);
  hm_slot<K, V>* data = (hm_slot<K, V>*)(malloc(sizeof(hm_slot<K, V>) * actual_capacity));
  // TODO: handle malloc errors

  // construct the free list
  for(size_t i = 0; i < actual_capacity; i++) {
    data[i].next = (i + 1) % actual_capacity;
    data[i].prev = (i + actual_capacity - 1) % actual_capacity;
    data[i].status = SLOT_STATUS_FREE;
    data[i].key = 0;
    data[i].value = 0;
  }

  hashmap<K, V, H> ht = {
    .m_cap = actual_capacity,
    .m_cnt = 0,
    .m_freelist = 0,
    .m_data = data,
    .m_hash = hash
  };

  return ht;
}

/**
 * Free hashmap
 */
template<class K, class V, class H> void hashmapfree(hashmap<K, V, H>* hm) {
  hm->m_cap = 0;
  hm->m_cnt = 0;
  hm->m_freelist = 0;
  free(hm->m_data);
}

/**
 * Resize the hashmap
 *
 * This can be called manually but will also be called internally whenever needed.
 * It might be useful to call this if you already know that you'll add a specific amount of items soon.
 */
template<class K, class V, class H> void hashmap<K, V, H>::resize(size_t new_size) {
  // TODO: implement
  printf("not implemented yet (resize)\n");
}

/**
 * Print debug information about the state of the hashmap
 */
template<class K, class V, class H> void hashmap<K, V, H>::dbg() {
  dbg(NULL);
}

/**
 * Print debug information about the state of the hashmap
 *
 * Includes a name which will be displayed alongside the debug information to tell debug logs apart.
 */
template<class K, class V, class H> void hashmap<K, V, H>::dbg(char* name) {
  if(name != NULL) { printf("dbg (%s):\n ", name); }
  else             { printf("dbg:\n");             }

  printf("- m_cap: %d\n", m_cap);
  printf("- m_cnt: %d\n", m_cnt);
  printf("- m_frl: %d\n", m_freelist);
  printf("- m_dat: [\n");
  for(size_t i = 0; i < m_cap; i++) {
    hm_slot<K, V> s = m_data[i];
    printf(
        "  (%2d) { .next = %2d, .prev = %2d, .status = %2d, .key = %2d, .value = %2d }\n",
        i, s.next, s.prev, s.status, s.key, s.value
    );
  }
  printf("]\n");
}

/**
 * Insert something into the hashmap
 *
 * If an element with the same key already exists it will be overwritten.
 * Handles hash collisions.
 */
template<class K, class V, class H> V hashmap<K, V, H>::ins(K key, V value) {

}

/**
 * Delete something from the hashmap by key
 *
 * Returns the value of the deleted element.
 * Handles hash collisions.
 */
template<class K, class V, class H> V hashmap<K, V, H>::del(K key) {

}

/**
 * Search for a value in the hashmap (by key)
 */
template<class K, class V, class H> V hashmap<K, V, H>::fnd(K key) {

}

/**
 * Search for a value in the hashmap by predicate (called with both key and value)
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> V hashmap<K, V, H>::fnd(F&& pred) {

}


/**
 * Check if the hashmap contains a value (by key)
 */
template<class K, class V, class H> bool hashmap<K, V, H>::has(K key) {

}


/**
 * Check if the hashtable contains a value by predicate lambda (called with both key and value)
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> bool hashmap<K, V, H>::has(F&& pred) {

}


/**
 * Iterate through all elements of the hashmap and call fn for every one of them (with both key and value)
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> void hashmap<K, V, H>::each(F&& fn) {

}


/**
 * Clear out all elements of the hashmap
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> void hashmap<K, V, H>::clr() {

}


/**
 * Filter hashmap elements.
 *
 * Takes a predicate lambda which decides which elements can stay and which get removed (called with both key and value).
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> void hashmap<K, V, H>::filter(F&& pred) {

}


/**
 * Merge another hashmap with the current one
 *
 * This goes through all elements of the other hashmap and adds the elements to the current one.
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> void hashmap<K, V, H>::cat(hashmap<K, V, H> other) {

}
