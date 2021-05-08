#include <cstdlib>
#include "common.hpp"

template<class T>
struct ht_data_slot {
  size_t prev;
  size_t next;
  char status;
  T value;
};

struct ht_free_slot {
  size_t prev;
  size_t next;
  char status;
};

template<class T>
union ht_slot {
  ht_data_slot<T> d;
  ht_free_slot f;
};

template<class T, class H> struct hashtable {
  size_t m_cap;
  size_t m_cnt;
  ht_free_slot* m_freelist;
  ht_slot<T>* m_slots;
  H m_hash;

  size_t cap() { return m_cap; }
  size_t cnt() { return m_cnt; }

  void resize(size_t new_size);

  T ins(T value);
  T del(T value);
  T fnd(T value);
  template<class F> T fnd(F&& pred);
  bool has(T value);
  template<class F> bool has(F&& fn);
  template<class F> void each(F&& fn);
  void clr();
  template<class F> void filter(F&& pred);
  void cat(hashtable<T, H> other);
};

template<class T, class H> hashtable<T, H> hashtableinit(size_t capacity, H hash) {

  // minimum capacity is 8 // TODO: should i change this to 16?
  size_t actual_capacity = max(capacity, 8);
  ht_slot<T>* data = (ht_slot<T>*)(malloc(sizeof(ht_slot<T>) * actual_capacity));
  // TODO: handle malloc errors

  ht_free_slot* freelist_head = NULL;

  // construct the free list
  for(size_t i = 0; i < actual_capacity; i++) {
    // TODO: implement
  }

  hashtable<T, H> ht = {
    .m_cap = actual_capacity,
    .m_cnt = 0,
    .m_freelist = freelist_head,
    .m_slots = data,
    .m_hash = hash
  };

  return ht;
}

template<class T, class H> T hashtable<T, H>::ins(T value) {
  return value;
}

template<class T, class H> T hashtable<T, H>::del(T value) {
  return value;
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


