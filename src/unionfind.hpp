#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include <cstdlib>

#ifndef eprintf
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#endif // eprintf

#ifndef fail
#define fail(...) { \
  eprintf(__VA_ARGS__); \
  exit(1); \
}
#endif // fail

// #define UF_DEBUG

struct unionfind {
  size_t* m_parent;
  size_t* m_rank;
  size_t  m_cnt;

  // TODO: find a solution to this: union is a reserved keyword
  bool _union(size_t a, size_t b);
  size_t find(size_t a);

  size_t non_destructive_find(size_t a);
  void dbg();
  void dbg(char* name);
};

unionfind unionfindinit(size_t cnt);
void unionfindfree(unionfind* uf);

#endif // UNIONFIND_H_

#ifdef UNIONFIND_IMPLEMENTATION

/**
 * TODO
 */
unionfind unionfindinit(size_t cnt) {
  size_t* parent = (size_t*)(malloc(sizeof(size_t) * cnt));
  size_t* rank   = (size_t*)(malloc(sizeof(size_t) * cnt));

  for(size_t i = 0; i < cnt; i++) {
    parent[i] = i;
    rank[i]   = 1;
  }

  unionfind uf = {
    .m_parent = parent,
    .m_rank   = rank,
    .m_cnt    = cnt
  };

  return uf;
}

/**
 * Free unionfind
 */
void unionfindfree(unionfind* uf) {
  free(uf->m_parent);
  free(uf->m_rank);
}

/**
 * Print debug information about the state of the union find structure
 */
void unionfind::dbg() {
  dbg(NULL);
}

/**
 * Print debug information about the state of the union find structure
 *
 * Includes a name which will be displayed alongside the debug information to tell debug logs apart.
 */
void unionfind::dbg(char* name) {
  if(name != NULL) { printf("dbg: (%s):\n", name); }
  else             { printf("dbg:\n");             }

  printf("- m_cnt: %d\n", m_cnt);
  printf("- elements: [\n");
  for(size_t i = 0; i < m_cnt; i++) {
    printf("    (%2d) { .parent = %2d, .rank = %2d }\n", i, m_parent[i], m_rank[i]);
  }
  printf("  ]\n");
}

/**
 * Find the partition (identified by a representative element) of the given element
 *
 * Time complexity: O(α(n)) amortized with α being the inverse Ackerman function (inverse of A(n, n)).
 * This can effectively be ignored as it's value is below 5 for almost all practical values of n.
 */
size_t unionfind::find(size_t a) {
  if(a >= m_cnt) fail("FATAL [unionfind] element out of range (el: %d, cnt: %d)", a, m_cnt);
  if(m_parent[a] == a) return a;

  size_t partition = find(m_parent[a]);
  m_parent[a] = partition;
  return partition;
}

/**
 * Like find but does not mutate the data structure
 *
 * Warning: only useful for testing, performance is bad
 */
size_t unionfind::non_destructive_find(size_t a) {
  if(a >= m_cnt) fail("FATAL [unionfind] element out of range (el: %d, cnt: %d)", a, m_cnt);
  if(m_parent[a] == a) return a;

  return find(m_parent[a]);
}

/**
 * Join two partitions together
 *
 * Can be called using any element of a partition, doesn't need to be the root element / representative element of the partition.
 *
 * Returns wether or not the two partitions did get joined together (only 'fails' if they already are the same)
 */
bool unionfind::_union(size_t a, size_t b) {
#ifdef UF_DEBUG
  dbg("union (before)");
#endif // UF_DEBUG
  size_t p_a = find(a);
  size_t p_b = find(b);

  if(p_a == p_b) return false;

  if(m_rank[p_a] > m_rank[p_b]) {
    m_parent[p_b] = p_a;
    m_parent[b]   = p_a;
  } else {
    m_parent[p_a] = p_b;
    m_parent[a]   = p_b;

    if(m_rank[a] == m_rank[b]) {
      m_rank[p_b]++;
    }
  }
#ifdef UF_DEBUG
  dbg("union (after)");
#endif // UF_DEBUG
  return true;
}

#endif // UNIONFIND_IMPLEMENTATION
