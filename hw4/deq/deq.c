#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

static void put(Rep r, End e, Data d) {
  if (e != Head && e != Tail) return;
  Node n=malloc(sizeof *n);
  if (!n) ERROR("malloc() failed");
  n->data=d;
  End opposite=1-e;
  n->np[e]=0;
  n->np[opposite]=r->ht[e];
  if (r->ht[e]) r->ht[e]->np[e]=n;
  else r->ht[opposite]=n;
  r->ht[e]=n;
  r->len++;
}

static Data ith(Rep r, End e, int i) {
  if (i < 0 || i >= r->len || (e != Head && e != Tail)) return 0;
  End opposite=1-e;
  Node current=r->ht[e];
  for (int iter=0; iter<i; iter++) current=current->np[opposite];
  return current->data;
}

static Data get(Rep r, End e) {
  if (r->len <= 0 || (e != Head && e != Tail)) return 0;
  Node return_node=r->ht[e];
  if (!return_node) return 0;
  End opposite=1-e;
  r->ht[e]=return_node->np[opposite];
  if (r->ht[e]) r->ht[e]->np[e]=0;
  else r->ht[opposite]=0;
  r->len--;
  Data data=return_node->data;
  free(return_node);
  return data;
}

static Data rem(Rep r, End e, Data d) {
  if (r->len <= 0 || (e != Head && e != Tail)) return 0;
  End opposite=1-e;
  Node current=r->ht[e];
  while (current->data != d) {
    if (!current->np[opposite]) return 0;
    current=current->np[opposite];
  }
  if (current == r->ht[Head]) return get(r,Head);
  if (current == r->ht[Tail]) return get(r,Tail);
  current->np[Head]->np[Tail]=current->np[Tail];
  current->np[Tail]->np[Head]=current->np[Head];
  r->len--;
  Data found=current->data;
  free(current);
  return found;
}

extern Deq deq_new() {
  Rep r=malloc(sizeof *r);
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }
extern void deq_head_put(Deq q, Data d) { put(rep(q),Head,d); }
extern Data deq_head_get(Deq q) { return get(rep(q),Head); }
extern Data deq_head_ith(Deq q, int i) { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }
extern void deq_tail_put(Deq q, Data d) { put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q) { return get(rep(q),Tail); }
extern Data deq_tail_ith(Deq q, int i) { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node current=rep(q)->ht[Head];
  while (current) {
    Node next=current->np[Tail];
    free(current);
    current=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  if (!s) ERROR("strdup() failed");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t;
    if (asprintf(&t,"%s%s%s",s,*s ? " " : "",d) < 0)
      ERROR("asprintf() failed");
    free(s);
    s=t;
    if (f) free(d);
  }
  return s;
}
