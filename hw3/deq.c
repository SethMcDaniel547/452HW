#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deq.h"
#include "error.h"

typedef enum { HEAD, TAIL } End;
typedef struct Node { struct Node *next, *prev; Data data; } Node;
typedef struct { Node *ends[2]; int len; } Rep;

/* Phase 0, task: provide the HW1 deque implementation locally. */
static Rep *rep(Deq q) { if (!q) ERROR("zero pointer"); return q; }

static void put(Rep *r, End e, Data d) {
  Node *n = malloc(sizeof *n);
  if (!n) ERROR("malloc() failed");
  n->data=d; n->next=n->prev=0;
  if (e == HEAD) {
    n->next=r->ends[HEAD];
    if (r->ends[HEAD]) r->ends[HEAD]->prev=n; else r->ends[TAIL]=n;
    r->ends[HEAD]=n;
  } else {
    n->prev=r->ends[TAIL];
    if (r->ends[TAIL]) r->ends[TAIL]->next=n; else r->ends[HEAD]=n;
    r->ends[TAIL]=n;
  }
  r->len++;
}

static Data get(Rep *r, End e) {
  Node *n=r->ends[e];
  if (!n) return 0;
  if (e == HEAD) {
    r->ends[HEAD]=n->next;
    if (r->ends[HEAD]) r->ends[HEAD]->prev=0; else r->ends[TAIL]=0;
  } else {
    r->ends[TAIL]=n->prev;
    if (r->ends[TAIL]) r->ends[TAIL]->next=0; else r->ends[HEAD]=0;
  }
  Data d=n->data; free(n); r->len--; return d;
}

static Data ith(Rep *r, End e, int i) {
  if (i < 0 || i >= r->len) return 0;
  Node *n=r->ends[e];
  while (i--) n=(e == HEAD) ? n->next : n->prev;
  return n->data;
}

static Data rem(Rep *r, End e, Data d) {
  Node *n=r->ends[e];
  while (n && n->data != d) n=(e == HEAD) ? n->next : n->prev;
  if (!n) return 0;
  if (n->prev) n->prev->next=n->next; else r->ends[HEAD]=n->next;
  if (n->next) n->next->prev=n->prev; else r->ends[TAIL]=n->prev;
  d=n->data; free(n); r->len--; return d;
}

extern Deq deq_new(void) { Rep *r=calloc(1,sizeof *r); if (!r) ERROR("malloc() failed"); return r; }
extern int deq_len(Deq q) { return rep(q)->len; }
extern void deq_head_put(Deq q, Data d) { put(rep(q),HEAD,d); }
extern Data deq_head_get(Deq q) { return get(rep(q),HEAD); }
extern Data deq_head_ith(Deq q,int i) { return ith(rep(q),HEAD,i); }
extern Data deq_head_rem(Deq q,Data d) { return rem(rep(q),HEAD,d); }
extern void deq_tail_put(Deq q,Data d) { put(rep(q),TAIL,d); }
extern Data deq_tail_get(Deq q) { return get(rep(q),TAIL); }
extern Data deq_tail_ith(Deq q,int i) { return ith(rep(q),TAIL,i); }
extern Data deq_tail_rem(Deq q,Data d) { return rem(rep(q),TAIL,d); }
extern void deq_map(Deq q,DeqMapF f) { for (Node *n=rep(q)->ends[HEAD]; n; n=n->next) f(n->data); }
extern void deq_del(Deq q,DeqMapF f) {
  Rep *r=rep(q); Node *n=r->ends[HEAD];
  while (n) { Node *next=n->next; if (f) f(n->data); free(n); n=next; }
  free(r);
}
extern Str deq_str(Deq q,DeqStrF f) {
  char *s=strdup(""); if (!s) ERROR("strdup() failed");
  for (Node *n=rep(q)->ends[HEAD]; n; n=n->next) {
    char *d=f ? f(n->data) : n->data, *t;
    if (asprintf(&t,"%s%s%s",s,*s ? " " : "",d) < 0) ERROR("asprintf() failed");
    free(s); if (f) free(d); s=t;
  }
  return s;
}
