#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];        // next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len;
} *Rep;

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

static void put(Rep r, End e, Data d) {
  Node n = malloc(sizeof(*n));
  if (!n) ERROR("malloc() failed");
  n->data = d;

  if (e == Head) {
    n->np[Head] = 0;
    n->np[Tail] = r->ht[Head];
    if (r->ht[Head]) {
      r->ht[Head]->np[Head] = n;
    } else {
      r->ht[Tail] = n;
    }
    r->ht[Head] = n;
  } else if (e == Tail) {
    n->np[Tail] = 0;
    n->np[Head] = r->ht[Tail];
    if (r->ht[Tail]) {
      r->ht[Tail]->np[Tail] = n;
    } else {
      r->ht[Head] = n;
    }
    r->ht[Tail] = n;
  }
  r->len++;
}

static Data ith(Rep r, End e, int i) { 
  if (i < 0 || i >= r->len) {
      return 0; 
  }
  int iter;
  if (e == Head) { //head
    Node current = r->ht[Head];
    for (iter = 0; iter < i; iter++) {
      current = current->np[Tail];
    }
    return current->data;
  } else if (e == Tail) { //tail
    Node current = r->ht[Tail];
    for (iter = 0; iter < i; iter++) {
      current = current->np[Head];
    }
    return current->data;
  } else {
    return 0;
  }
}

static Data get(Rep r, End e) {
  if (r->len <= 0 || (e != Head && e != Tail)) {
    return 0; 
  }

  Node returnNode = r->ht[e];
  if (!returnNode) {
    return 0;
  }

  End oppositeEnd = 1 - e;
  r->ht[e] = returnNode->np[oppositeEnd];

  if (r->ht[e]) {
    r->ht[e]->np[e] = NULL; //removes the new head node's head pointer
  } else {
    r->ht[oppositeEnd] = NULL; //if the queue is now empty, remove the pointer on the otherside
  }

  r->len--;
  Data d = returnNode->data;
  free(returnNode);
  return d;
}

static Data rem(Rep r, End e, Data d) {
  if (r->len <= 0 || (e != Head && e != Tail)) {
    return 0; 
  }
  End oppositeEnd = 1 - e; 


  //Loop through from selected end to find equal node data
  Node current = r->ht[e];
  while (current->data != d) {
    if (current->np[oppositeEnd] == NULL) {
      return 0;
    }
    current = current->np[oppositeEnd];
  }

  //Found the correct node
  //check if end? if so use get function
  if (current == r->ht[Head]) {
    return get(r, Head);
  } else if (current == r->ht[Tail]) {
    return get(r, Tail);
  }

  //if in mid
  //Fix head and tail of current
  current->np[Head]->np[Tail] = current->np[Tail];
  current->np[Tail]->np[Head] = current->np[Head];

  r->len--;
  Data foundData = current->data;
  free(current);
  return foundData;
}

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head);   }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail);   }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) {
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}