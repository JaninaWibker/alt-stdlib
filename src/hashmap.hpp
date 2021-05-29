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
  size_t actual_capacity = MAX(capacity, 8);
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

  hashmap<K, V, H> hm = {
    .m_cap = actual_capacity,
    .m_cnt = 0,
    .m_freelist = 0,
    .m_data = data,
    .m_hash = hash
  };

  return hm;
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

  size_t new_cap = -1;

  // grow if >= 50% filled (double size)
  if(2 * new_size >= m_cap     ) { new_cap = m_cap * 2; }
  // shrink if <= 12.5% filled (half size)
  else if(new_size <= m_cap / 8) { new_cap = m_cap / 2; }

  if(new_cap != -1) {
    auto hm = hashmapinit<K, V, H>(new_size, m_hash);

    each([&hm](K key, V value) { hm.ins(key, value); });

    free(m_data);
    m_cap = hm.m_cap;
    m_cnt = hm.m_cnt;
    m_freelist = hm.m_freelist;
    m_data = hm.m_data;
  }
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
  printf("  ]\n");
}

/**
 * Insert something into the hashmap
 *
 * If an element with the same key already exists it will be overwritten.
 * Handles hash collisions.
 */
template<class K, class V, class H> V hashmap<K, V, H>::ins(K key, V value) {

  int hash = m_hash(key) % m_cap;

#ifdef HT_DEBUG
  printf("hash: %d, key: %d (ins)\n", hash, value);
#endif

  hm_slot<K, V> s = m_data[hash];

  resize(m_cnt + 1);

  if((s.status & SLOT_STATUS_FREE) != 0) {

    // adjust the head of the freelist if necessary
    if(m_freelist == hash) { m_freelist = m_data[m_freelist].next; }

    // adjust free list
    m_data[s.next].prev = s.prev;
    m_data[s.prev].next = s.next;

    // update slot
    m_data[hash].next = -1;
    m_data[hash].prev = -1;
    m_data[hash].status = SLOT_STATUS_HEAD;
    m_data[hash].key = key;
    m_data[hash].value = value;

    m_cnt++;

#ifdef HM_DEBUG
    dbg();
#endif

  } else {

    // found head of linked list
    if((m_data[hash].status & SLOT_STATUS_HEAD) != 0) {
      // get free slot using freelist
      size_t target = m_freelist;
      size_t old_freelist_prev = m_data[m_freelist].prev;
      m_freelist = m_data[m_freelist].next;
      m_data[m_freelist].prev = old_freelist_prev;
      m_data[old_freelist_prev].next = m_freelist;

      // populate slot
      m_data[target].next = -1;
      m_data[target].prev = hash;
      m_data[target].status = 0;
      m_data[target].key = key;
      m_data[target].value = value;

      // update m_data[hash] and it's linked list members if necessary
      if(m_data[hash].next != -1) {
        m_data[m_data[hash].next].prev = target;
        m_data[target].next = m_data[hash].next;
      }
      m_data[hash].next = target;

      m_cnt++;

#ifdef HM_DEBUG
      dbg();
#endif

    // found element with incorrect hash
    } else {
      // move contents of m_data[hash] to another free slot
      size_t target = m_freelist;
      m_freelist = m_data[m_freelist].next;

      // update freelist prev and next here
      m_data[m_data[target].next].prev = m_data[target].prev;
      m_data[m_data[target].prev].next = m_data[target].next;

      m_data[target].next = m_data[hash].next;
      m_data[target].prev = m_data[hash].prev;
      m_data[target].status = 0;
      m_data[target].key = m_data[hash].key;
      m_data[target].value = m_data[hash].value;

      if(m_data[target].next != -1) {
        m_data[m_data[target].next].prev = target;
      }
      m_data[m_data[target].prev].next = target;


      // use m_data[hash] for new key & value
      m_data[hash].next = -1;
      m_data[hash].prev = -1;
      m_data[hash].status = SLOT_STATUS_HEAD;
      m_data[hash].key = key;
      m_data[hash].value = value;

      m_cnt++;

#ifdef HT_DEBUG
      dbg();
#endif

    }
  }

  return value;
}

/**
 * Delete something from the hashmap by key
 *
 * Returns the value of the deleted element.
 * Handles hash collisions.
 */
template<class K, class V, class H> V hashmap<K, V, H>::del(K key) {

  int hash = m_hash(key) % m_cap;

  if((m_data[hash].status & SLOT_STATUS_HEAD) == 0) { return NULL; }

  size_t s = hash;
  bool found = false;

  while(s != -1) {
    if(m_data[s].key == key) {
      found = true;
      break;
    }
    s = m_data[s].next;
  }

  if(!found) { return NULL; }

  size_t freelist_target = -1;

  V rtn_value = m_data[s].value;

  // not end of linked list
  if(m_data[s].next != -1) {

    size_t next = m_data[s].next;

    // is head of linked list -> swap value the with following slot
    if((m_data[s].status & SLOT_STATUS_HEAD) != 0) {
      m_data[s].key = m_data[next].key;
      m_data[s].value = m_data[next].value;
    } else {
      next = s;
      s = m_data[s].prev;
    }

    // the list looks like this: <a>, s, next, <b> and needs to
    // look like this: <a>, s, <b> so next needs to be cut out

    if(m_data[next].next != -1) {
      m_data[m_data[next].next].prev = s;
    }
    m_data[s].next = m_data[m_data[s].next].next;

    // add m_data[next] to the freelist
    freelist_target = next;

  // end of linked list
  } else {
    // decouple m_data[s] from linked list and add it to the freelist
    if(m_data[s].prev != -1) {
      m_data[m_data[s].prev].next = -1;
      m_data[s].prev = -1;
      m_data[s].next = -1;
    }
    freelist_target = s;
  }

  // update freelist using freelist_target

  m_data[freelist_target].status = SLOT_STATUS_FREE;
  m_data[freelist_target].key = 0;
  m_data[freelist_target].value = 0;

  // set as first freelist element
  m_data[freelist_target].next = m_freelist;
  m_data[freelist_target].prev = m_data[m_freelist].prev;
  m_data[m_data[m_freelist].prev].next = freelist_target;
  m_data[m_freelist].prev = freelist_target;

  m_freelist = freelist_target;

  m_cnt--;

#ifdef HT_DEBUG
  dbg();
#endif

  resize(m_cnt);

  return rtn_value;
}

/**
 * Search for a value in the hashmap (by key)
 */
template<class K, class V, class H> V hashmap<K, V, H>::fnd(K key) {

  int hash = m_hash(key) % m_cap;

  if((m_data[hash].status & SLOT_STATUS_HEAD) == 0) { return false; }

  size_t s = hash;
  bool found = false;

  while(s != -1) {
    if(m_data[s].key == key) {
      return m_data[s].value;
    }
    s = m_data[s].next;
  }

  return NULL;

}

/**
 * Search for a value in the hashmap by predicate (called with both key and value)
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> V hashmap<K, V, H>::fnd(F&& pred) {

  for(size_t i = 0; i < m_cap; i++) {
    if((m_data[i].status & SLOT_STATUS_FREE) != 0) { continue; }

    if(pred(m_data[i].key, m_data[i].value)) {
      return m_data[i].value;
    }
  }

  return NULL;

}


/**
 * Check if the hashmap contains a value (by key)
 */
template<class K, class V, class H> bool hashmap<K, V, H>::has(K key) {

  int hash = m_hash(key) % m_cap;

  if((m_data[hash].status & SLOT_STATUS_HEAD) == 0) { return false; }

  size_t s = hash;
  bool found = false;

  while(s != -1) {
    if(m_data[s].key == key) { return true; }

    s = m_data[s].next;
  }

  return false;
}


/**
 * Check if the hashmap contains a value by predicate lambda (called with both key and value)
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> bool hashmap<K, V, H>::has(F&& pred) {

  for(size_t i = 0; i < m_cap; i++) {
    if((m_data[i].status & SLOT_STATUS_FREE) != 0) { continue; }

    if(pred(m_data[i].key, m_data[i].value)) { return true; }
  }

  return false;
}


/**
 * Iterate through all elements of the hashmap and call fn for every one of them (with both key and value)
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> void hashmap<K, V, H>::each(F&& fn) {

  for(size_t i = 0; i < m_cap; i++) {
    if((m_data[i].status & SLOT_STATUS_FREE) != 0) { continue; }

    fn(m_data[i].key, m_data[i].value);
  }

}


/**
 * Clear out all elements of the hashmap
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> void hashmap<K, V, H>::clr() {

  for(size_t i = 0; i < m_cap; i++) {
    m_data[i].next = (i + 1) % m_cap;
    m_data[i].prev = (i + m_cap - 1) % m_cap;
    m_data[i].status = SLOT_STATUS_FREE;
    m_data[i].key = 0;
    m_data[i].value = 0;
  }
}


/**
 * Filter hashmap elements.
 *
 * Takes a predicate lambda which decides which elements can stay and which get removed (called with both key and value).
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> template<class F> void hashmap<K, V, H>::filter(F&& pred) {

  for(size_t i = 0; i < m_cap; i++) {
    if((m_data[i].status % SLOT_STATUS_FREE) != 0) { continue; }

    if(!pred(m_data[i].key, m_data[i].value)) { del(m_data[i].key); }
  }
}


/**
 * Merge another hashmap with the current one
 *
 * This goes through all elements of the other hashmap and adds the elements to the current one.
 *
 * Warning: Expensive action
 */
template<class K, class V, class H> void hashmap<K, V, H>::cat(hashmap<K, V, H> other) {

  for(size_t i = 0; i < other.m_cap; i++) {
    if((other.m_data[i].status % SLOT_STATUS_FREE) != 0) { continue; }

    ins(other.m_data[i].key, other.m_data[i].value);
  }
}

