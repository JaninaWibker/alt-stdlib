#include <cstdlib>
#include <assert.h>

#define max(a, b) \
   ({ __typeof__ (a) _a = (a); \
      __typeof__ (b) _b = (b); \
      _a > _b ? _a : _b; })

#define min(a, b) \
  ({ __typeof__ (a) _a = (a); \
     __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

template<class T> struct array {
  size_t m_len;
  size_t m_cap;
  T* m_data;

  size_t len() { return m_len; }
  size_t cap() { return m_cap; }

  void resize(size_t new_size);
  T put(size_t idx, T value);
  T at(size_t idx);
  T ins(size_t idx, T value);
  T del(size_t idx);
  T push(T value);
  T pop();
  size_t fnd(T target);
  template<class F> size_t fnd(F&& pred);
  bool has(T target);
  template<class F> bool has(F&& pred);

  void cat(array<T> other);
  void rev();
  void clr();
  void fill(T value);
  void fill(T value, size_t limit);
  void slice(size_t start, size_t end);
  template<class F> void sort(F&& comp);   // TODO: implement
  void sort();
  template<class F> void each(F&& fn);
  template<class F> void map(F&& fn);      // TODO: should this be done in place? what should the return type be? how can I retrieve the return type of fn for this?
  template<class F> void fold(F&& fn);     // TODO: implement
  template<class F> void filter(F&& pred);
};

template<class T> static array<T> arrayinit(size_t capacity) {

  size_t actual_capacity = max(capacity, 1);
  T* data = (T*)(malloc(sizeof(T) * actual_capacity));
  // TODO: handle malloc errors

  for(size_t i = 0; i < actual_capacity; i++) { data[i] = 0; }

  array<T> arr = {
    .m_len = 0,
    .m_cap = actual_capacity,
    .m_data = data
  };

  return arr;
}

template<class T> void array<T>::resize(size_t new_size) {
  if(new_size >= m_cap) {
    m_cap = m_cap * 2;
    m_data = (T*)(realloc(m_data, sizeof(T) * m_cap));
  } else if(new_size <= m_cap / 4) {
    m_cap = m_cap / 2;
    m_data = (T*)(realloc(m_data, sizeof(T) * m_cap));
  }
}

template<class T> T array<T>::put(size_t idx, T value) {
  if(m_cap <= idx) { return 0; }
  if(m_len <= idx) {
    m_len = idx + 1;
    resize(m_len);
  }

  return m_data[idx] = value;
}

template<class T> T array<T>::at(size_t idx) {
  assert(idx < m_len);
  return m_data[idx];
}

template<class T> T array<T>::ins(size_t idx, T value) {
  assert(idx < m_cap);
  if(m_len <= idx) {
    m_len = idx + 1;
    resize(m_len);
    m_data[idx] = value;
    return value;
  }

  for(size_t i = m_len; i > idx; i--) {
    m_data[i] = m_data[i-1];
  }

  m_data[idx] = value;
  m_len++;
  return value;
}

template<class T> T array<T>::del(size_t idx) {
  assert(idx < m_cap);
  assert(idx < m_len);

  T rtn = m_data[idx];

  for(size_t i = idx; i < m_len - 1; i++) {
    m_data[i] = m_data[i + 1];
  }

  m_data[--m_len] = 0;
  resize(m_len);
  return rtn;
}

template<class T> T array<T>::push(T value) {
  resize(m_len + 1);

  return m_data[m_len++] = value;
}

template<class T> T array<T>::pop() {
  resize(m_len - 1);

  T rtn = m_data[m_len - 1];

  m_data[--m_len] = 0;

  return rtn;
}

template<class T> size_t array<T>::fnd(T target) {

  for(size_t i = 0; i < m_len; i++) {
    if(m_data[i] == target) { return i; }
  }

  return -1;
}

template<class T> template<class F> size_t array<T>::fnd(F&& pred) {

  for(size_t i = 0; i < m_len; i++) {
    if(pred(m_data[i], i)) { return i; }
  }

  return -1;
}

template<class T> bool array<T>::has(T target) {
  return fnd(target) != -1;
}

template<class T> template<class F> bool array<T>::has(F&& pred) {
  return fnd(pred) != -1;
}

template<class T> void array<T>::cat(array<T> other) {
  resize(m_len + other.m_len);

  for(size_t i = 0; i < other.m_len; i++) {
    m_data[m_len + i] = other.m_data[i];
  }

  m_len = m_len + other.m_len;
}

template<class T> void array<T>::rev() {
  for(size_t i = 0; i < m_len / 2; i++) {
    T tmp = m_data[m_len - 1 - i];
    m_data[m_len - 1 - i] = m_data[i];
    m_data[i] = tmp;
  }
}

template<class T> void array<T>::clr() {
  for(size_t i = 0; i < m_len; i++) {
    m_data[i] = 0;
  }

  m_len = 0;
  resize(m_len);
}

template<class T> void array<T>::fill(T value, size_t limit) {
  for(size_t i = 0; i < limit; i++) {
    m_data[i] = value;
  }
}

template<class T> void array<T>::fill(T value) {
  fill(value, m_len);
}

template<class T> void array<T>::slice(size_t start, size_t end) {

  // support using negative numbers as an offset starting from the end of the array
  if(start <= -1) { start == m_len - start + 1; }
  if(end   <= -1) { end   == m_len - end   + 1; }

  assert(start >= 0);
  assert(start <= m_len);
  assert(end <= m_len);
  assert(start <= end);

  // move wanted data to the beginning of the array
  for(size_t i = start; i < end; i++) {
    m_data[i - start] = m_data[i];
  }

  // clear out unwanted data
  for(size_t i = end - start; i < m_len; i++) {
    m_data[i] = 0;
  }

  // adjust the length and resize if needed
  m_len = end - start;
  resize(m_len);
}

template<class T> void array<T>::sort() {
  sort([](T a, T b) { return a - b; });
}

template<class T> template<class F> void array<T>::sort(F&& comp) {
  // TODO: implement
}

template<class T> template<class F> void array<T>::each(F&& fn) {
  for(size_t i = 0; i < m_len; i++) {
    fn(m_data[i], i);
  }
}

template<class T> template<class F> void array<T>::filter(F&& pred) {
  size_t j = 0;
  size_t k = 0;
  for(; k < m_len; k++) {
    if(pred(m_data[k], k)) {
      m_data[j++] = m_data[k];
    }
  }

  m_len = j;

  resize(m_len);
}
