#ifndef UNIONFIND_H_
#define UNIONFIND_H_

#include <cstdlib>

// #define UF_DEBUG

struct unionfind {
  size_t* m_parent;
  size_t* m_rank;
  size_t  m_cnt;

  // TODO: find a solution to this: union is a reserved keyword
  void _union(size_t a, size_t b);
  size_t find(size_t a);
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
 * TODO
 */
void unionfindfree(unionfind* uf) {
  free(uf->m_parent);
  free(uf->m_rank);
}

/**
 * TODO
 */
size_t unionfind::find(size_t a) {
  if(m_parent[a] == a) return a;

  size_t partition = find(m_parent[a]);
  m_parent[a] = partition;
  return partition;
}

/**
 * TODO
 */
void unionfind::_union(size_t a, size_t b) {
  size_t p_a = find(a);
  size_t p_b = find(b);

  if(p_a == p_b) return;

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
}

#endif // UNIONFIND_IMPLEMENTATION
