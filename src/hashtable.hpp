#include <cstdlib>
#include "common.hpp"

#define HT_DEBUG

// the slot is part of the free list
#define SLOT_STATUS_FREE 0x01
// the data is at the correct slot hash-wise (head of linked list)
#define SLOT_STATUS_HEAD 0x02

template<class T>
struct ht_slot {
  size_t prev;
  size_t next;
  char status;
  T value;
};

template<class T, class H> struct hashtable {
  size_t m_cap;
  size_t m_cnt;
  size_t m_freelist;
  ht_slot<T>* m_data;
  H m_hash;

  size_t cap() { return m_cap; }
  size_t cnt() { return m_cnt; }

  void resize(size_t new_size);

  T ins(T value);
  bool del(T value);
  T fnd(T value);
  template<class F> T fnd(F&& pred);
  bool has(T value);
  template<class F> bool has(F&& fn);
  template<class F> void each(F&& fn);
  void clr();
  template<class F> void filter(F&& pred);
  void cat(hashtable<T, H> other);

  void dbg();
  void dbg(char* name);
};

template<class T, class H> hashtable<T, H> hashtableinit(size_t capacity, H hash) {

  // minimum capacity is 8 // TODO: should i change this to 16?
  size_t actual_capacity = max(capacity, 8);
  ht_slot<T>* data = (ht_slot<T>*)(malloc(sizeof(ht_slot<T>) * actual_capacity));
  // TODO: handle malloc errors

  // construct the free list
  for(size_t i = 0; i < actual_capacity; i++) {
    data[i].next = (i + 1) % actual_capacity;
    data[i].prev = (i + actual_capacity - 1) % actual_capacity;
    data[i].status = SLOT_STATUS_FREE;
    data[i].value = 0;
  }

  hashtable<T, H> ht = {
    .m_cap = actual_capacity,
    .m_cnt = 0,
    .m_freelist = 0,
    .m_data = data,
    .m_hash = hash
  };

  return ht;
}

template<class T, class H> void hashtable<T, H>::resize(size_t new_size) {
  // TODO: need to handle the case that the list is full
  printf("not implemented yet (resize)\n");
}

template<class T, class H> void hashtable<T, H>::dbg() {
  dbg(NULL);
}

template<class T, class H> void hashtable<T, H>::dbg(char* name) {
  if(name != NULL) { printf("dbg (%s):\n ", name); }
  else             { printf("dbg:\n");             }

  printf("- m_cap: %d\n", m_cap);
  printf("- m_cnt: %d\n", m_cnt);
  printf("- m_frl: %d\n", m_freelist);
  printf("- m_dat: [\n");
  for(size_t i = 0; i < m_cap; i++) {
    ht_slot<T> s = m_data[i];
    printf("  (%2d) { .next = %2d, .prev = %2d, .status = %2d, .value = %d }\n", i, s.next, s.prev, s.status, s.value);
  }
  printf("]\n");
}

template<class T, class H> T hashtable<T, H>::ins(T value) {

  int hash = m_hash(value) % m_cap;

  printf("hash: %d, value: %d (ins)\n", hash, value);

  ht_slot<T> s = m_data[hash];

  resize(m_cnt + 1);

  // is first element with hash
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
    m_data[hash].value= value;

    m_cnt++;

#ifdef HT_DEBUG
    dbg();
#endif

  // add linked list element
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
      m_data[target].value = value;

      // update m_data[hash] and it's linked list members if necessary
      if(m_data[hash].next != -1) {
        m_data[m_data[hash].next].prev = target;
        m_data[target].next = m_data[hash].next;
      }
      m_data[hash].next = target;

      m_cnt++;

#ifdef HT_DEBUG
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
      m_data[target].value = m_data[hash].value;

      if(m_data[target].next != -1) {
        m_data[m_data[target].next].prev = target;
      }
      m_data[m_data[target].prev].next = target;


      // use m_data[hash] for new value
      m_data[hash].next = -1;
      m_data[hash].prev = -1;
      m_data[hash].status = SLOT_STATUS_HEAD;
      m_data[hash].value = value;

      m_cnt++;

#ifdef HT_DEBUG
      dbg();
#endif
    }
  }

  return value;
}

template<class T, class H> bool hashtable<T, H>::del(T value) {

  // TODO: DEL IS FOR NOW ONLY BASED ON THE HASH; NOT THE VALUE
  int hash = m_hash(value) % m_cap;

  if((m_data[hash].status & SLOT_STATUS_HEAD) == 0) { return false; }

  size_t s = hash;
  bool found = false;

  while(s != -1) {
    if(m_data[s].value == value) {
      found = true;
      break;
    }
    s = m_data[s].next;
  }

  if(!found) { return false; }

  size_t freelist_target = -1;

  // not end of linked list
  if(m_data[s].next != -1) {

    size_t next = m_data[s].next;

    // is head of linked list -> swap value the with following slot
    if((m_data[s].status & SLOT_STATUS_HEAD) != 0) {
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
  m_data[freelist_target].value = 0;

  // set as first freelist element
  m_data[freelist_target].next = m_freelist;
  m_data[freelist_target].prev = m_data[m_freelist].prev;
  m_data[m_data[m_freelist].prev].next = freelist_target;
  m_data[m_freelist].prev = freelist_target;

  m_freelist = freelist_target;

  m_cnt--;

  dbg();

  resize(m_cnt);

  return true;
}

template<class T, class H> T hashtable<T, H>::fnd(T value) {
  return value;
}

template<class T, class H> template<class F> T hashtable<T, H>::fnd(F&& pred) {
  return NULL;
}

template<class T, class H> bool hashtable<T, H>::has(T value) {
  return false;
}

template<class T, class H> template<class F> bool hashtable<T, H>::has(F&& pred) {
  return false;
}

template<class T, class H> template<class F> void hashtable<T, H>::each(F&& fn) {

}

template<class T, class H> void hashtable<T, H>::clr() {

}

template<class T, class H> template<class F> void hashtable<T, H>::filter(F&& pred) {

}

template<class T, class H> void hashtable<T, H>::cat(hashtable<T, H> other) {

}


